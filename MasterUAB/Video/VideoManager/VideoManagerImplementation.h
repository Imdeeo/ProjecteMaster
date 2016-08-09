#ifndef H_VIDEO_MANAGER_IMPLEMENTATION_H
#define H_VIDEO_MANAGER_IMPLEMENTATION_H

#include "VideoManager.h"
#include "SDL.h"
#include <string>

class CVideoManagerImplementation : public IVideoManager
{	
public:
	CVideoManagerImplementation();
	virtual ~CVideoManagerImplementation();
	bool LoadClip(const std::string &l_ClipFile, bool _Restart = false);
	TheoraVideoClip* LoadVideoClip(const std::string &l_ClipFile, bool _Restart = false);
	TheoraVideoClip* GetClip(const std::string &l_ClipName);
	bool ClearClip(const std::string &l_ClipFile);
	bool PlayClip(const std::string &l_ClipName);
	bool RenderSceenClip(const std::string &l_ClipName);
	void Update(float _ElapsedTime);
	TheoraVideoClip* GetActualClip();
};

#endif //H_VIDEO_MANAGER_IMPLEMENTATION_H