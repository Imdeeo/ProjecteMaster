#ifndef H_VIDEO_MANAGER_IMPLEMENTATION_H
#define H_VIDEO_MANAGER_IMPLEMENTATION_H

#include "VideoManager.h"
#include "SDL.h"

class CVideoManagerImplementation : public IVideoManager
{	
public:
	CVideoManagerImplementation();
	virtual ~CVideoManagerImplementation();
	bool Init();
	void Update(float _ElapsedTime);
	TheoraVideoClip* GetClip();
};

#endif //H_VIDEO_MANAGER_IMPLEMENTATION_H