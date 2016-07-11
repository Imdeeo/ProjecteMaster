#include "VideoManagerImplementation.h"
#include "Utils.h"

CVideoManagerImplementation::CVideoManagerImplementation()
{

}

CVideoManagerImplementation::~CVideoManagerImplementation()
{
}

bool CVideoManagerImplementation::Init()
{
	/* Initialize the SDL library */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,	"Couldn't initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	/* Clean up on exit */
	atexit(SDL_Quit);

	/*
	 * Initialize the display in a 640x480 8-bit palettized mode,
	 * requesting a software surface
	 */
	SDL_Window *sdlWindow;
	SDL_Renderer *sdlRenderer;
	SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &sdlWindow, &sdlRenderer);

	if (sdlWindow == NULL || sdlRenderer == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(sdlRenderer);
	SDL_RenderPresent(sdlRenderer);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(sdlRenderer, 640, 480);

	SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		640, 480);

	return true;
}