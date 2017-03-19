#ifndef H_BUTON_H
#define H_BUTON_H

#include "Utils.h"

class SpriteInfo;

class CButon 
{
private:
public:
	CButon(SpriteInfo* _normal, SpriteInfo* _highlight, SpriteInfo* _pressed);
	virtual ~CButon();
	UAB_BUILD_GET_SET(SpriteInfo*, Normal)
	UAB_BUILD_GET_SET(SpriteInfo*, Highlight)
	UAB_BUILD_GET_SET(SpriteInfo*, Pressed)
};

#endif //H_BUTON_H