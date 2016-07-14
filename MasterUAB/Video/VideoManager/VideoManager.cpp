#include "VideoManager.h"
#include "VideoManagerImplementation.h"

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#if !defined(_LARGEFILE_SOURCE)
#define _LARGEFILE_SOURCE
#endif
#if !defined(_LARGEFILE64_SOURCE)
#define _LARGEFILE64_SOURCE
#endif
#if !defined(_FILE_OFFSET_BITS)
#define _FILE_OFFSET_BITS 64
#endif

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef _REENTRANT
# define _REENTRANT
#endif

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include "theora\theoradec.h"
#include "vorbis/codec.h"
#include <SDL.h>

/* yes, this makes us OSS-specific for now. None of SDL, libao, libao2
give us any way to determine hardware timing, and since the
hard/kernel buffer is going to be most of or > a second, that's
just a little bit important */
#if defined(__FreeBSD__)
#include <machine/soundcard.h>
#define AUDIO_DEVICE "/dev/audio"
#elif defined(__NetBSD__) || defined(__OpenBSD__)
#include <soundcard.h>
#define AUDIO_DEVICE "/dev/audio"
#else
#include <winscard.h>
#include <dsound.h>
#define AUDIO_DEVICE "/dev/dsp"
#endif
#include <winioctl.h>

/* Helper; just grab some more compressed bitstream and sync it for
page extraction */
int buffer_data(FILE *in, ogg_sync_state *oy){
	char *buffer = ogg_sync_buffer(oy, 4096);
	int bytes = fread(buffer, 1, 4096, in);
	ogg_sync_wrote(oy, bytes);
	return(bytes);
}

/* never forget that globals are a one-way ticket to Hell */
/* Ogg and codec state for demux/decode */
ogg_sync_state   oy;
ogg_page         og;
ogg_stream_state vo;
ogg_stream_state to;
th_info      ti;
th_comment   tc;
th_dec_ctx       *td;
th_setup_info    *ts;
vorbis_info      vi;
vorbis_dsp_state vd;
vorbis_block     vb;
vorbis_comment   vc;
th_pixel_fmt     px_fmt;

int              theora_p = 0;
int              vorbis_p = 0;
int              stateflag = 0;

/* SDL Video playback structures */
SDL_Surface *screen;
SDL_Texture *yuv_overlay;
SDL_Rect rect;

/* single frame video buffering */
int          videobuf_ready = 0;
ogg_int64_t  videobuf_granulepos = -1;
double       videobuf_time = 0;

/* single audio fragment audio buffering */
int          audiobuf_fill = 0;
int          audiobuf_ready = 0;
ogg_int16_t *audiobuf;
ogg_int64_t  audiobuf_granulepos = 0; /* time position of last sample */

/* audio / video synchronization tracking:

Since this will make it to Google at some point and lots of people
search for how to do this, a quick rundown of a practical A/V sync
strategy under Linux [the UNIX where Everything Is Hard].  Naturally,
this works on other platforms using OSS for sound as well.

In OSS, we don't have reliable access to any precise information on
the exact current playback position (that, of course would have been
too easy; the kernel folks like to keep us app people working hard
doing simple things that should have been solved once and abstracted
long ago).  Hopefully ALSA solves this a little better; we'll probably
use that once ALSA is the standard in the stable kernel.

We can't use the system clock for a/v sync because audio is hard
synced to its own clock, and both the system and audio clocks suffer
from wobble, drift, and a lack of accuracy that can be guaranteed to
add a reliable percent or so of error.  After ten seconds, that's
100ms.  We can't drift by half a second every minute.

Although OSS can't generally tell us where the audio playback pointer
is, we do know that if we work in complete audio fragments and keep
the kernel buffer full, a blocking select on the audio buffer will
give us a writable fragment immediately after playback finishes with
it.  We assume at that point that we know the exact number of bytes in
the kernel buffer that have not been played (total fragments minus
one) and calculate clock drift between audio and system then (and only
then).  Damp the sync correction fraction, apply, and walla: A
reliable A/V clock that even works if it's interrupted. */

long         audiofd_totalsize = -1;
int          audiofd_fragsize;      /* read and write only complete fragments
									so that SNDCTL_DSP_GETOSPACE is
									accurate immediately after a bank
									switch */
int          audiofd = -1;
ogg_int64_t  audiofd_timer_calibrate = -1;

/* get relative time since beginning playback, compensating for A/V
drift */
double get_time(){
	static ogg_int64_t last = 0;
	static ogg_int64_t up = 0;
	ogg_int64_t now;
	struct timeval tv;
	
	gettimeofday(&tv, 0);
	now = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	if (audiofd_timer_calibrate == -1)audiofd_timer_calibrate = last = now;

	if (audiofd<0){
		/* no audio timer to worry about, we can just use the system clock */
		/* only one complication: If the process is suspended, we should
		reset timing to account for the gap in play time.  Do it the
		easy/hack way */
		if (now - last>1000)audiofd_timer_calibrate += (now - last);
		last = now;
	}

	if (now - up>200){
		double timebase = (now - audiofd_timer_calibrate)*.001;
		int hundredths = timebase * 100 - (long)timebase * 100;
		int seconds = (long)timebase % 60;
		int minutes = ((long)timebase / 60) % 60;
		int hours = (long)timebase / 3600;

		fprintf(stderr, "   Playing: %d:%02d:%02d.%02d                       \r",
			hours, minutes, seconds, hundredths);
		up = now;
	}

	return (now - audiofd_timer_calibrate)*.001;

}


/* clean quit on Ctrl-C for SDL and thread shutdown as per SDL example
(we don't use any threads, but libSDL does) */
int got_sigint = 0;
static void sigint_handler(int signal) {
	got_sigint = 1;
}

static void open_video(void){
	int w;
	int h;
	w = (ti.pic_x + ti.frame_width + 1 & ~1) - (ti.pic_x&~1);
	h = (ti.pic_y + ti.frame_height + 1 & ~1) - (ti.pic_y&~1);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window *sdlWindow;
	SDL_Renderer *sdlRenderer;
	SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &sdlWindow, &sdlRenderer);

	if (sdlWindow == NULL) {
		fprintf(stderr, "Unable to set %dx%d video: %s\n", w, h, SDL_GetError());
		exit(1);
	}

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(sdlRenderer);
	SDL_RenderPresent(sdlRenderer);

	if (px_fmt == TH_PF_422)
	{
		yuv_overlay = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_YUY2, SDL_TEXTUREACCESS_STREAMING, 640, 480);
		SDL_UpdateYUVTexture();
	}
	else
	{
		yuv_overlay = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, 640, 480);
		SDL_UpdateYUVTexture();
	}

	if (yuv_overlay == NULL) {
		fprintf(stderr, "SDL: Couldn't create SDL_yuv_overlay: %s\n",
			SDL_GetError());
		exit(1);
	}
	rect.x = 0;
	rect.y = 0;
	rect.w = w;
	rect.h = h;

	SDL_RenderCopy(sdlRenderer, yuv_overlay, NULL, &rect);
	SDL_RenderPresent(sdlRenderer);
}

/* dump the theora (or vorbis) comment header */
static int dump_comments(th_comment *tc){
	int i, len;
	char *value;
	FILE *out = stdout;

	fprintf(out, "Encoded by %s\n", tc->vendor);
	if (tc->comments){
		fprintf(out, "theora comment header:\n");
		for (i = 0; i<tc->comments; i++){
			if (tc->user_comments[i]){
				len = tc->comment_lengths[i];
				value = malloc(len + 1);
				memcpy(value, tc->user_comments[i], len);
				value[len] = '\0';
				fprintf(out, "\t%s\n", value);
				free(value);
			}
		}
	}
	return(0);
}

/* Report the encoder-specified colorspace for the video, if any.
We don't actually make use of the information in this example;
a real player should attempt to perform color correction for
whatever display device it supports. */
static void report_colorspace(th_info *ti)
{
	switch (ti->colorspace){
	case TH_CS_UNSPECIFIED:
		/* nothing to report */
		break;;
	case TH_CS_ITU_REC_470M:
		fprintf(stderr, "  encoder specified ITU Rec 470M (NTSC) color.\n");
		break;;
	case TH_CS_ITU_REC_470BG:
		fprintf(stderr, "  encoder specified ITU Rec 470BG (PAL) color.\n");
		break;;
	default:
		fprintf(stderr, "warning: encoder specified unknown colorspace (%d).\n",
			ti->colorspace);
		break;;
	}
}

/* helper: push a page into the appropriate steam */
/* this can be done blindly; a stream won't accept a page
that doesn't belong to it */
static int queue_page(ogg_page *page){
	if (theora_p)ogg_stream_pagein(&to, page);
	if (vorbis_p)ogg_stream_pagein(&vo, page);
	return 0;
}

static void usage(void){
	fprintf(stderr,
		"Usage: player_example <file.ogv>\n"
		"input is read from stdin if no file is passed on the command line\n"
		"\n"
		);
}

IVideoManager *IVideoManager::InstantiateVideoManager()
{
	return new CVideoManagerImplementation();
}