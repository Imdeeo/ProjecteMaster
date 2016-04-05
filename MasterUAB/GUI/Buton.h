#ifndef H_BUTON_H
#define H_BUTON_H

#include "Utils.h"
#include "Sprite.h"

class CButon 
{
private:
public:
	UAB_BUILD_GET_SET(CSprite*, Normal)
	UAB_BUILD_GET_SET(CSprite*, Highlight)
	UAB_BUILD_GET_SET(CSprite*, Pressed)
};

#endif //H_BUTON_H