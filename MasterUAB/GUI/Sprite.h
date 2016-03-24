#ifndef H_SPRITE_H
#define H_SPRITE_H

#include "SpriteMap.h"
#include "Utils.h"

class CSprite
{
private:
public:
	UAB_BUILD_GET_SET(CSpriteMap*, SpriteMap)
	UAB_BUILD_GET_SET(int, IzqAbajo)
	UAB_BUILD_GET_SET(int, IzqArriba)
	UAB_BUILD_GET_SET(int, DerAbajo)
	UAB_BUILD_GET_SET(int, DerArriba)
};

#endif //H_SPRITE_H