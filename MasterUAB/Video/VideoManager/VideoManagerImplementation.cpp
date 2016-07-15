#include "VideoManagerImplementation.h"
#include "Utils.h"

CVideoManagerImplementation::CVideoManagerImplementation()
{

}

CVideoManagerImplementation::~CVideoManagerImplementation()
{
	CHECKED_DELETE(mgr);
}

bool CVideoManagerImplementation::Init()
{
	mgr = new TheoraVideoManager();
	clip = mgr->createVideoClip("Data\\Video\\file.ogg");
	return true;
}
 
void CVideoManagerImplementation::Update(float _ElapsedTime)
{
	TheoraVideoFrame *frame = clip->getNextFrame();
	if (frame)
	{
		// transfer the frame pixels to your display device, texure, graphical context or whatever you use.
		clip->popFrame(); // be sure to pop the frame from the frame queue when you're done
	}
	mgr->update(_ElapsedTime);
}