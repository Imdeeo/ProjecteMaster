#ifndef H_GUI_MANAGER_H
#define H_GUI_MANAGER_H

#include <string>
#include <stdlib.h>
#include <d3d11.h>
#include <vector>
#include <map>

class CButon;
class CSlider;
class CMaterial;
class CRenderableVertex;
class CColor;
class CRenderManager;

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
	Vect4f color;
};

class CGUIManager 
{
private:
	std::string m_ActiveItem;
	std::string m_HotItem;
	std::vector<CRenderableVertex> m_VertexBuffers;
	std::vector<CMaterial> m_Materials;
	std::vector<GUICommand> m_Commands;	
	std::map<std::string, SpriteMapInfo> m_SpriteMaps; 
	std::map<std::string, SpriteInfo> m_Sprites;
	std::map<std::string, CButon> m_Buttons;
	std::map<std::string, CSlider> m_Sliders;
	

public:

	void SetActive(const std::string& id);
	void SetNotActive(const std::string& id);
	void SetHot(const std::string& id);
	void SetNotHot(const std::string& id);
	void Render(CRenderManager *RenderManager);
};

#endif //H_GUI_MANAGER_H