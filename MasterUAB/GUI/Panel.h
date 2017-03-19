#ifndef H_Panel_H
#define H_Panel_H

#include "Utils.h"
#include "GUIManager.h"

class SpriteInfo;

class CPanel
{
private:
public:
	CPanel(SpriteInfo* _normal);
	~CPanel();
	UAB_BUILD_GET_SET(SpriteInfo*, Normal)
};

#endif //H_Panel_H