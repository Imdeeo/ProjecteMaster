#include "VideoManager.h"
#include "VideoManagerImplementation.h"

IVideoManager *IVideoManager::InstantiateVideoManager()
{
	return new CVideoManagerImplementation();
}