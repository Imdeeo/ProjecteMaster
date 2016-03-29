#include "GUIManager.h"
#include "Materials\Material.h"
#include "Buton.h"
#include "Slider.h"
#include "RenderManager\RenderManager.h"
#include "RenderableObjects\VertexTypes.h"
#include "Materials\Material.h"
#include "Effects\EffectManager.h"
#include <assert.h>
#include "RenderableObjects\RenderableVertexs.h"

CGUIManager::CGUIManager()
{
	m_ActiveItem = "";
	m_HotItem = "";

}

CGUIManager::~CGUIManager()
{

}

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

bool CGUIManager::Load(CXMLTreeNode *TreeNode)
{

}

bool CGUIManager::DoButton(const std::string& guiID, const std::string& buttonID, const GUIPosition& position)
{
	GUICommand command = { 
		m_Buttons[buttonID]->GetNormal(),
		position.x, position.y, position.x + position.width, position.y + position.height,
		0, 0, 1, 1,
		Vect4f(1, 1, 1, 1) };
	m_Commands.push_back(command);
}

SliderResult CGUIManager::DoSlider(const std::string& guiID, const std::string& sliderID, const GUIPosition& position, float minValue, float maxValue, float currentValue)
{

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
				m_Materials[currentSpriteMap->MaterialIndex]->Apply(technique);

				m_VertexBuffers[currentSpriteMap->MaterialIndex]->UpdateVertexs(currentBufferData, currentVertex);
				m_VertexBuffers[currentSpriteMap->MaterialIndex]->Render(&RenderManager, technique, &CEffectManager::m_SceneParameters, currentVertex);
			}
			currentVertex = 0;
			currentSpriteMap = commandSpriteMap;
		}
		float x1 = (command.x1 / (screenWidth * 0.5f)) - 1.0f;
		float x2 = (command.x2 / (screenWidth * 0.5f)) - 1.0f;
		float y1 = 1.0f - (command.y1 / (screenHeight * 0.5f));
		float y2 = 1.0f - (command.y2 / (screenHeight * 0.5f));

		float u1 = commandSprite->u1 * (1.0f - command.u1) + commandSprite->u2 * command.u1;
		float u2 = commandSprite->u1 * (1.0f - command.u2) + commandSprite->u2 * command.u2;
		float v1 = commandSprite->v1 * (1.0f - command.v1) + commandSprite->v2 * command.v1;
		float v2 = commandSprite->v1 * (1.0f - command.v2) + commandSprite->v2 * command.v2;

		currentBufferData[currentVertex++] = { Vect4f(x1, y2, 0.f, 1.f), command.color, Vect2f(u1, v2) };
		currentBufferData[currentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
		currentBufferData[currentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), command.color, Vect2f(u1, v1) };
	
		currentBufferData[currentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), command.color, Vect2f(u1, v1) };
		currentBufferData[currentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
		currentBufferData[currentVertex++] = { Vect4f(x2, y1, 0.f, 1.f), command.color, Vect2f(u2, v1) };
	}
	if (currentVertex > 0)
	{
		m_Materials[currentSpriteMap->MaterialIndex]->Apply(technique);

		m_VertexBuffers[currentSpriteMap->MaterialIndex]->UpdateVertexs(currentBufferData, currentVertex);
		m_VertexBuffers[currentSpriteMap->MaterialIndex]->Render(&RenderManager, technique, &CEffectManager::m_SceneParameters, currentVertex);
	}
}