#ifndef H_VIDEO_MANAGER_IMPLEMENTATION_H
#define H_VIDEO_MANAGER_IMPLEMENTATION_H

#include "VideoManager.h"
#include "SDL.h"
#include "theora.h"

class CVideoManagerImplementation : public IVideoManager
{	
public:
	CVideoManagerImplementation();
	virtual ~CVideoManagerImplementation();
	bool Init();
};

#endif //H_VIDEO_MANAGER_IMPLEMENTATION_H