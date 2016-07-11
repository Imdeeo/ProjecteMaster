#include "VideoManager.h"
#include "VideoManagerImplementation.h"


IVideoManager *IVideoManager::InstantiateSoundManager()
{
	return new CVideoManagerImplementation();
}