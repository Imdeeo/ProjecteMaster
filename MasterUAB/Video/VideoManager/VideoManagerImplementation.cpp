#include "VideoManagerImplementation.h"
#include "Utils.h"

CVideoManagerImplementation::CVideoManagerImplementation()
{
	mgr = new theoraplayer::Manager();	
}

CVideoManagerImplementation::~CVideoManagerImplementation()
{
	CHECKED_DELETE(mgr);
}

bool CVideoManagerImplementation::Init()
{
	clip = mgr->createVideoClip("short.ogg");
	return true;
}
 
void CVideoManagerImplementation::Update(float _ElapsedTime)
{
	mgr->update(_ElapsedTime);
	theoraplayer::VideoFrame *frame = clip->fetchNextFrame();
	if (frame)
	{
		// transfer the frame pixels to your display device, texure, graphical context or whatever you use.
		clip->popFrame(); // be sure to pop the frame from the frame queue when you're done
	}

}