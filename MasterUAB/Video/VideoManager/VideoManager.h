#ifndef H_VIDEO_MANAGER_H
#define H_VIDEO_MANAGER_H
#include <string>

class TheoraVideoClip;
class TheoraVideoManager;

class IVideoManager
{
public:
	static IVideoManager* InstantiateVideoManager();
	virtual ~IVideoManager() {}
	virtual bool LoadClip(const std::string &l_ClipFile, bool _Restart = false) = 0;
	virtual TheoraVideoClip* LoadVideoClip(const std::string &l_ClipFile, bool _Restart = false)=0;
	virtual TheoraVideoClip* GetClip(const std::string &l_ClipName) = 0;
	virtual bool ClearClip(const std::string &l_ClipFile) = 0;
	virtual bool PlayClip(const std::string &l_ClipName) = 0;
	virtual bool RenderSceenClip(const std::string &l_ClipName) = 0;
	virtual void Update(float _ElapsedTime) = 0;
	virtual TheoraVideoClip *GetActualClip()= 0;
	virtual void ReloadClip(const std::string &l_ClipName)= 0;
	TheoraVideoManager* mgr;
protected:
	IVideoManager()
	{}
	TheoraVideoClip* m_ActualClip;
	std::string m_Path;
};

#endif //H_VIDEO_MANAGER_H