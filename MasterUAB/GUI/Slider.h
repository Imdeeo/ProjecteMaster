#ifndef H_SLIDER_H
#define H_SLIDER_H

#include "Utils.h"
#include "GUIManager.h"

class CSlider
{
private:
public:
	CSlider(SpriteInfo* _Base, SpriteInfo* _Top, SpriteInfo* _Handle, SpriteInfo* PressedHandle);
	~CSlider();
	UAB_BUILD_GET_SET(SpriteInfo*, Base)
	UAB_BUILD_GET_SET(SpriteInfo*, Top)
	UAB_BUILD_GET_SET(SpriteInfo*, Handle)
	UAB_BUILD_GET_SET(SpriteInfo*, PressedHandle)
	float handleRelativeWidth;
	float handleRelativeHeight;
};

#endif //H_SLIDER_H