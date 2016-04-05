#ifndef H_SPRITE_MAP_H
#define H_SPRITE_MAP_H

#include "Utils.h"

class CSpriteMap
{
private:
public:
	UAB_BUILD_GET_SET(size_t, Indice);
	UAB_BUILD_GET_SET(int, Width);
	UAB_BUILD_GET_SET(int, Height);
};

#endif //H_SPRITE_MAP_H