#include "VideoManagerImplementation.h"
#include "Utils.h"

CVideoManagerImplementation::CVideoManagerImplementation()
{
	mgr = new TheoraVideoManager();
}

CVideoManagerImplementation::~CVideoManagerImplementation()
{
	CHECKED_DELETE(mgr);
}

bool CVideoManagerImplementation::Init()
{
	clip = mgr->createVideoClip("Data\\Video\\file.ogg");
	return true;
}
 
void CVideoManagerImplementation::Update(float _ElapsedTime)
{
	mgr->update(_ElapsedTime);
	TheoraVideoFrame *frame = clip->getNextFrame();
	if (frame)
	{
		// transfer the frame pixels to your display device, texure, graphical context or whatever you use.
		clip->popFrame(); // be sure to pop the frame from the frame queue when you're done
	}

}