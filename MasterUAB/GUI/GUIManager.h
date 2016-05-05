#ifndef H_GUI_MANAGER_H
#define H_GUI_MANAGER_H

#include <string>
#include <stdlib.h>
#include <d3d11.h>
#include <vector>
#include <map>
#include <unordered_map>
#include "Utils.h"
#include "Math\Color.h"
#include "RenderableObjects\VertexTypes.h"

class CButon;
class CSlider;
class CMaterial;
class CRenderableVertexs;
class CRenderManager;
class CXMLTreeNode;
class CGUIPosition;

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

struct FontChar
{
	uint16 x, y, width, height;
	int16 xoffset, yoffset, xadvance;
	uint8 page, chnl;
};


struct SliderResult
{
	float real;
	float temp;
};

class CGUIManager 
{
private:
	MV_POSITION4_COLOR_TEXTURE_VERTEX m_CurrentBufferData[MAX_VERTICES_PER_CALL];
	std::string m_ActiveItem;
	std::string m_HotItem;
	std::string m_SelectedItem;
	std::vector<CRenderableVertexs*> m_VertexBuffers;
	std::vector<CMaterial*> m_Materials;
	std::map<std::string, SpriteMapInfo> m_SpriteMaps; 
	std::map<std::string, SpriteInfo> m_Sprites;
	std::map<std::string, CButon*> m_Buttons;
	std::map<std::string, CSlider*> m_Sliders;
	std::vector<GUICommand> m_Commands;
	std::string m_FileName;
	std::unordered_map< std::string, int16 > m_LineHeightPerFont;
	std::unordered_map< std::string, int16 > m_BasePerFont;
	std::unordered_map< std::string, std::unordered_map< wchar_t, FontChar > > m_CharactersPerFont;
	std::unordered_map< std::string, std::unordered_map< wchar_t, std::unordered_map< wchar_t, int > > > m_KerningsPerFont;
	std::unordered_map< std::string, std::vector<SpriteInfo*> > m_TexturePerFont;

public:
	enum GUICoordType
	{
		GUI_ABSOLUTE,
		GUI_RELATIVE,
		GUI_RELATIVE_WIDTH,
		GUI_RELATIVE_HEIGHT
	};

	enum GUIAnchor
	{
		TOP = 0x1,
		MID = 0x2,
		BOTTOM = 0x4,

		LEFT = 0x10,
		CENTER = 0x20,
		RIGHT = 0x40,

		TOP_LEFT = TOP | LEFT,
		TOP_CENTER = TOP | CENTER,
		TOP_RIGHT = TOP | RIGHT,
		MID_LEFT = MID | LEFT,
		MID_CENTER = MID | CENTER,
		MID_RIGHT = MID | RIGHT,
		BOTTOM_LEFT = BOTTOM | LEFT,
		BOTTOM_CENTER = BOTTOM | CENTER,
		BOTTOM_RIGHT = BOTTOM | RIGHT,
	};

	CGUIManager();
	virtual ~CGUIManager();
	void CheckInput();
	bool Load(std::string _FileName);
	void SetActive(const std::string& id);
	void SetNotActive();
	void SetHot(const std::string& id);
	void SetNotHot(const std::string& id);
	void SetSelected(const std::string& id);
	void SetNotSelected(const std::string& id);
	
	void Render(CRenderManager *RenderManager);

	bool DoButton(const std::string& guiID, const std::string& buttonID, const CGUIPosition& position);
	//SliderResult DoSlider(const std::string& guiID, const std::string& sliderID, const GUIPosition& position, float minValue, float maxValue, float currentValue);
	int FillCommandQueueWithTextAux(const std::string& _font, const std::string& _text, 
		const CColor& _color = CColor(1, 1, 1, 1), Vect4f *textBox_ = nullptr);
	void FillCommandQueueWithText(const std::string& _font, const std::string& _text,
		Vect2f coord, GUIAnchor anchor = GUIAnchor::BOTTOM_LEFT, const CColor& _color = CColor(1, 1, 1, 1));
	std::string DoTextBox(const std::string& guiID, const std::string& _font, const std::string& currentText, CGUIPosition position);


	UAB_BUILD_GET_SET(bool, InputUpToDate)
	UAB_BUILD_GET_SET(int, MouseX)
	UAB_BUILD_GET_SET(int, MouseY)
	UAB_BUILD_GET_SET(bool, MouseWentPressed)
	UAB_BUILD_GET_SET(bool, MouseWentReleased)
};

#endif //H_GUI_MANAGER_H