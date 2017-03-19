#ifndef H_GUI_POSITION_H
#define H_GUI_POSITION_H

#include "Utils.h"
#include "GUIManager.h"

class SpriteInfo;

class CGUIPosition 
{
private:
public:
	CGUIPosition(float _x, float _y, float _w, float _h, 
		CGUIManager::GUIAnchor _anchor = CGUIManager::GUIAnchor::TOP_LEFT,
		CGUIManager::GUICoordType _anchorCoordsType = CGUIManager::GUICoordType::GUI_ABSOLUTE,
		CGUIManager::GUICoordType _sizeCorrdsType = CGUIManager::GUICoordType::GUI_ABSOLUTE);
	virtual ~CGUIPosition();
	
	UAB_BUILD_GET_SET(int, x)
	UAB_BUILD_GET_SET(int, y)
	UAB_BUILD_GET_SET(int, width)
	UAB_BUILD_GET_SET(int, height)
};

#endif //H_GUI_POSITION_H 
