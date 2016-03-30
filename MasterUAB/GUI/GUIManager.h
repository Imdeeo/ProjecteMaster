#ifndef H_GUI_MANAGER_H
#define H_GUI_MANAGER_H

#include <string>
#include <stdlib.h>
#include <d3d11.h>
#include <vector>
#include <map>
#include "Utils.h"
#include "Math\Color.h"

class CButon;
class CSlider;
class CMaterial;
class CRenderableVertexs;
class CRenderManager;
class CXMLTreeNode;

struct SpriteMapInfo
{
	int MaterialIndex;
	int w, h;
};

struct SpriteInfo
{
	SpriteMapInfo *SpriteMap;
	float u1, u2, v1, v2;
};

struct GUICommand
{
	SpriteInfo *sprite;
	int x1, y1, x2, y2; //top left is 0,0
	float u1, v1, u2, v2;
	CColor color;
};


struct SliderResult
{
	float real;
	float temp;
};


struct GUIPosition
{
	int x;
	int y;
	int width;
	int height;
};

class CGUIManager 
{
private:
	std::string m_ActiveItem;
	std::string m_HotItem;
	std::vector<CRenderableVertexs*> m_VertexBuffers;
	std::vector<CMaterial*> m_Materials;
	std::map<std::string, SpriteMapInfo> m_SpriteMaps; 
	std::map<std::string, SpriteInfo> m_Sprites;
	std::map<std::string, CButon*> m_Buttons;
	std::map<std::string, CSlider*> m_Sliders;
	std::vector<GUICommand> m_Commands;
	std::string m_FileName;

public:
	CGUIManager();
	virtual ~CGUIManager();
	void CheckInput();
	bool Load(std::string _FileName);
	void SetActive(const std::string& id);
	void SetNotActive();
	void SetHot(const std::string& id);
	void SetNotHot(const std::string& id);
	void Render(CRenderManager *RenderManager);
	bool DoButton(const std::string& guiID, const std::string& buttonID, const GUIPosition& position);
	//SliderResult DoSlider(const std::string& guiID, const std::string& sliderID, const GUIPosition& position, float minValue, float maxValue, float currentValue);
	UAB_BUILD_GET_SET(bool, InputUpToDate)
	UAB_BUILD_GET_SET(int, MouseX)
	UAB_BUILD_GET_SET(int, MouseY)
	UAB_BUILD_GET_SET(bool, MouseWentPressed)
	UAB_BUILD_GET_SET(bool, MouseWentReleased)
};

#endif //H_GUI_MANAGER_H