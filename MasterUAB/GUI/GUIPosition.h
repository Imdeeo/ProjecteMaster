#ifndef H_GUI_POSITION_H
#define H_GUI_POSITION_H

#include "Utils.h"
#include "GUIManager.h"

class CXMLTreeNode;
class SpriteInfo;

class CGUIPosition 
{
private:
public:
	CGUIPosition(float _x, float _y, float _w, float _h, 
		GUIAnchor _anchor = GUIAnchor::TOP_LEFT,
		GUICoordType _anchorCoordsType = GUICoordType::GUI_ABSOLUTE,
		GUICoordType _sizeCorrdsType = GUICoordType::GUI_ABSOLUTE);
	virtual ~CGUIPosition();
	
	UAB_BUILD_GET_SET(int, x)
	UAB_BUILD_GET_SET(int, y)
	UAB_BUILD_GET_SET(int, width)
	UAB_BUILD_GET_SET(int, height)
};

#endif //H_GUI_POSITION_H 
