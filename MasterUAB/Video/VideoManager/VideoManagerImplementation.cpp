#include "VideoManagerImplementation.h"
#include "Utils.h"
#include "Engine\UABEngine.h"
#include <theoraplayer/TheoraPlayer.h>
#include <theoraplayer/TheoraDataSource.h>

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
	clip = mgr->createVideoClip("media/bunny.ogv");
	if (clip == nullptr)
		return false;
	clip->setAutoRestart(true);
	return true;
}
 
void CVideoManagerImplementation::Update(float _ElapsedTime)
{
	mgr->update(_ElapsedTime);
}

TheoraVideoClip* CVideoManagerImplementation::GetClip()
{
	return clip;
}