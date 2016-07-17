#ifndef H_VIDEO_MANAGER_IMPLEMENTATION_H
#define H_VIDEO_MANAGER_IMPLEMENTATION_H

#include "VideoManager.h"
#include "SDL.h"
#include "theoraplayer\Manager.h"

class CVideoManagerImplementation : public IVideoManager
{	
public:
	CVideoManagerImplementation();
	virtual ~CVideoManagerImplementation();
	bool Init();
	void Update(float _ElapsedTime);
	theoraplayer::Manager* mgr;
	theoraplayer::VideoClip* clip;
};

#endif //H_VIDEO_MANAGER_IMPLEMENTATION_H