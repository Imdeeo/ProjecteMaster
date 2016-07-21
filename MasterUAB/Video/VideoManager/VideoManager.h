#ifndef H_VIDEO_MANAGER_H
#define H_VIDEO_MANAGER_H

class TheoraVideoClip;
class TheoraVideoManager;

class IVideoManager
{
public:
	static IVideoManager* InstantiateVideoManager();
	virtual ~IVideoManager() {}
	virtual bool Init() = 0;
	virtual void Update(float _ElapsedTime) = 0;
	virtual TheoraVideoClip *GetClip()= 0;
	TheoraVideoManager* mgr;
protected:
	IVideoManager()
	{}
	TheoraVideoClip* clip;
};

#endif //H_VIDEO_MANAGER_H