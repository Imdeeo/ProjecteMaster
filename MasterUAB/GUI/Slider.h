#ifndef H_SLIDER_H
#define H_SLIDER_H

#include "Utils.h"
#include "Sprite.h"

class CSlider
{
private:
public:
	UAB_BUILD_GET_SET(CSprite*, Base)
	UAB_BUILD_GET_SET(CSprite*, Top)
	UAB_BUILD_GET_SET(CSprite*, Handle)
	UAB_BUILD_GET_SET(CSprite*, PressedHandle)
};

#endif //H_SLIDER_H