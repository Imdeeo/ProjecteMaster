#ifndef H_VIDEO_MANAGER_H
#define H_VIDEO_MANAGER_H

class IVideoManager
{
public:
	static IVideoManager* InstantiateVideoManager();
	virtual ~IVideoManager() {}
	virtual bool Init() = 0;
protected:
	IVideoManager()
	{}
};

#endif //H_VIDEO_MANAGER_H