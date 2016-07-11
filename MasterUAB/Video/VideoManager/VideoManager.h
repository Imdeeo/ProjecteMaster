#ifndef H_VIDEO_MANAGER_H
#define H_VIDEO_MANAGER_H

class IVideoManager
{
public:
	static IVideoManager* InstantiateSoundManager();

	virtual ~IVideoManager() {}

protected:
	IVideoManager()
	{}
};

#endif //H_VIDEO_MANAGER_H