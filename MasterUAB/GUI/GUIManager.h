#ifndef H_GUI_MANAGER_H
#define H_GUI_MANAGER_H

#include <string>
#include <stdlib.h>
#include <d3d11.h>
#include <vector>
#include <map>

class CSpriteMap;
class CSprite;
class CButon;
class CSlider;
class CMaterial;


class CGUIManager 
{
private:
	std::string m_ActiveItem;
	std::string m_HotItem;
	std::vector<CMaterial> m_Material;
	std::vector<ID3D11Buffer> m_VertexBuffer;
	std::map<std::string, CSpriteMap> m_SpriteMaps; 
	std::map<std::string, CSprite> m_Sprites;
	std::map<std::string, CButon> m_Buttons;
	std::map<std::string, CSlider> m_Sliders;

public:
	void SetActive(const std::string& id);
	void SetNotActive(const std::string& id);
	void SetHot(const std::string& id);
	void SetNotHot(const std::string& id);
};

#endif //H_GUI_MANAGER_H