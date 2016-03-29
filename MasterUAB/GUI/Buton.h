#ifndef H_BUTON_H
#define H_BUTON_H

#include "Utils.h"
#include "GUIManager.h"

class CButon 
{
private:
public:
	UAB_BUILD_GET_SET(SpriteInfo*, Normal)
	UAB_BUILD_GET_SET(SpriteInfo*, Highlight)
	UAB_BUILD_GET_SET(SpriteInfo*, Pressed)
};

#endif //H_BUTON_H