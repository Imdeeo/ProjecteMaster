#include "GUIManager.h"
#include "Materials\Material.h"
#include "SpriteMap.h"
#include "Sprite.h"
#include "Buton.h"
#include "Slider.h"

void CGUIManager::SetActive(const std::string& id)
{
	m_ActiveItem = id;
}

void CGUIManager::SetNotActive(const std::string& id)
{
	m_ActiveItem = "";
}

void CGUIManager::SetHot(const std::string& id)
{
	if (m_ActiveItem == "" || m_ActiveItem == id)
	{
		m_HotItem = id;
	}
}

void CGUIManager::SetNotHot(const std::string& id)
{
	if (m_HotItem == id)
	{
		m_HotItem = "";
	}
}