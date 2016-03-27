#include "GUIManager.h"
#include "Materials\Material.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "Buton.h"
#include "Slider.h"
#include "RenderManager\RenderManager.h"
#include "RenderableObjects\VertexTypes.h"
#include "Materials\Material.h"

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

void CGUIManager::Render(CRenderManager *RenderManager)
{
	MV_POSITION4_COLOR_TEXTURE_VERTEX currentBufferData[s_MaxVerticesPerCall];
	int currentVertex = 0;
	SpriteMapInfo *currentSpriteMap = nullptr;
	for (int i = 0; i < commandsExecutionOrder.size(); ++i)
	{
		GUICommand &command = m_Commands[commandsExecutionOrder[i]];
		assert(command.x1 <= command.x2);
		assert(command.y2 <= command.y2);

		SpriteInfo *commandSprite = command.sprite;
		SpriteMapInfo *commandSpriteMap = commandSprite->SpriteMap;

		if (currentSpriteMap != commandSpriteMap || currentVertex == s_MaxverticesPerCall)
		{
			if (currentVertex > 0)
			{
				//TODO log a warning if we get here by "currentVertex == s_MaxVerticesPerCall"
				//TODO draw all c urrent vertex in the currentBuffer
			}
			currentVertex = 0;
			currentSpriteMap = commandSpriteMap;
		}
	
	}
}