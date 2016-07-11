#include "VideoManagerImplementation.h"

CVideoManagerImplementation::CVideoManagerImplementation()
{

}

CVideoManagerImplementation::~CVideoManagerImplementation()
{
}

bool CVideoManagerImplementation::Init()
{
	SDL_VideoInit("");
}