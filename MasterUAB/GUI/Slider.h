#ifndef H_SLIDER_H
#define H_SLIDER_H

#include "Utils.h"
#include "GUIManager.h"

class CSlider
{
private:
public:
	UAB_BUILD_GET_SET(SpriteInfo*, Base)
	UAB_BUILD_GET_SET(SpriteInfo*, Top)
	UAB_BUILD_GET_SET(SpriteInfo*, Handle)
	UAB_BUILD_GET_SET(SpriteInfo*, PressedHandle)
};

#endif //H_SLIDER_H