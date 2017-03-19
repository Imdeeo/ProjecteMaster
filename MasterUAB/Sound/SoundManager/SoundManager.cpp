#include "SoundManager.h"
#include "SoundManagerImplementation.h"


ISoundManager *ISoundManager::InstantiateSoundManager()
{
	return new CSoundManagerImplementation();
}