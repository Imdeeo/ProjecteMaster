#include "GUIManager.h"
#include "Materials\Material.h"
#include "Buton.h"
#include "Slider.h"
#include "RenderManager\RenderManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Materials\Material.h"
#include "Effects\EffectManager.h"
#include <assert.h>
#include "RenderableObjects\RenderableVertexs.h"
#include "InputManager\InputManager.h"
#include "ContextManager\ContextManager.h"
#include "XML\XMLTreeNode.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"


CGUIManager::CGUIManager()
{
	m_ActiveItem = "";
	m_HotItem = "";
	m_InputUpToDate = false;
	m_MouseWentPressed = false;
	m_MouseWentReleased = false;
}

CGUIManager::~CGUIManager()
{

}

void CGUIManager::SetActive(const std::string& id)
{
	m_ActiveItem = id;
}

void CGUIManager::SetNotActive()
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

bool CGUIManager::Load(std::string _FileName)
{
	m_FileName = _FileName;
	SpriteMapInfo l_spriteMapinfo;
	SpriteInfo l_sprite;
	CXMLTreeNode l_XML;
	int u1, u2, v1, v2;
	if (l_XML.LoadFile(_FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["gui_elements"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("gui_spritemap"))
				{
					std::string l_sprtieMapinfoname = l_Element.GetPszProperty("name");
					l_spriteMapinfo = { m_Materials.size(), l_Element.GetIntProperty("width"), l_Element.GetIntProperty("height") };
					m_SpriteMaps[l_sprtieMapinfoname] = l_spriteMapinfo;
					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						CXMLTreeNode l_aux = l_Element(i);
						if (l_aux.GetName() == std::string("material"))
						{
							m_Materials.push_back(new CMaterial(l_aux));
							m_VertexBuffers.push_back(new CUABTrianglesListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(m_CurrentBufferData, MAX_VERTICES_PER_CALL, MAX_VERTICES_PER_CALL/3, false));
						}
						else if (l_aux.GetName() == std::string("sprite"))
						{
							u1 = l_aux.GetIntProperty("x");
							u2 = u1 + l_aux.GetIntProperty("w");
							v1 = l_aux.GetIntProperty("y");
							v2 = u1 + l_aux.GetIntProperty("h");
							l_sprite = { &m_SpriteMaps[l_sprtieMapinfoname], u1, u2, v1, v2 };
						
							m_Sprites[l_aux.GetPszProperty("name")] = l_sprite;
						}
					}
				
				}
				else if (l_Element.GetName() == std::string("button"))
				{
					m_Buttons[l_Element.GetPszProperty("name")] = new CButon(&m_Sprites[l_Element.GetPszProperty("normal")], &m_Sprites[l_Element.GetPszProperty("highlight")], &m_Sprites[l_Element.GetPszProperty("pressed")]);
				}
				else if (l_Element.GetName() == std::string("slider"))
				{
					
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

void CGUIManager::CheckInput()
{
	if (!m_InputUpToDate)
	{
		CInputManager* l_InputManager = CInputManager::GetInputManager();
		m_MouseX = l_InputManager->GetCursor().x;
		m_MouseY = l_InputManager->GetCursor().y;

		m_MouseWentPressed = m_MouseX = l_InputManager->IsActionActive("MOUSE_CLICK_LEFT");
		m_MouseWentReleased = !m_MouseWentPressed;

		m_InputUpToDate = true;
	}
}

bool IsMouseInside(int _mouseX, int _mouseY, int x, int y, int width, int height)
{
	if (_mouseX >= x && _mouseX <= x + width)
	{
		if (_mouseY >= y && _mouseY <= x + height)
		{
			return true;
		}
	}
	return false;
}

bool CGUIManager::DoButton(const std::string& guiID, const std::string& buttonID/*, const GUIPosition& position*/)
{
	GUIPosition position = { 10, 10, 50, 50 };
	CheckInput();
	bool l_result = false;
	SpriteInfo* l_sprite = l_sprite = m_Buttons[buttonID]->GetNormal();
	if (m_ActiveItem == guiID)
	{
		if (m_MouseWentReleased)
		{
			if (m_HotItem == guiID)
			{
				l_result = true;
			}
			SetNotActive();
		}	
	}
	else if (m_HotItem == guiID)
	{
		if (m_MouseWentPressed)
		{
			SetActive(guiID);
			l_sprite = m_Buttons[buttonID]->GetPressed();
		}
	}

	if (IsMouseInside(m_MouseX, m_MouseY, position.x, position.y, position.width, position.height))
	{
		SetHot(guiID);
		l_sprite = m_Buttons[buttonID]->GetHighlight();
	}
	else
	{
		SetNotHot(guiID);
	}

	GUICommand command = { 
		l_sprite,
		position.x, position.y, position.x + position.width, position.y + position.height,
		0, 0, 1, 1,
		CColor(1, 1, 1, 1) };
	m_Commands.push_back(command);
	
	return l_result;
}



/*SliderResult CGUIManager::DoSlider(const std::string& guiID, const std::string& sliderID, const GUIPosition& position, float minValue, float maxValue, float currentValue)
{

}*/


void CGUIManager::Render(CRenderManager *RenderManager)
{
	
	int currentVertex = 0;
	SpriteMapInfo *currentSpriteMap = nullptr;
	for (int i = 0; i < m_Commands.size(); ++i)  //commandsExecutionOrder.size()
	{
		GUICommand &command = m_Commands[i];
		assert(command.x1 <= command.x2);
		assert(command.y2 <= command.y2);

		SpriteInfo *commandSprite = command.sprite;
		SpriteMapInfo *commandSpriteMap = commandSprite->SpriteMap;

		if (currentSpriteMap != commandSpriteMap || currentVertex == MAX_VERTICES_PER_CALL)
		{
			if (currentVertex > 0)
			{
				//TODO log a warning if we get here by "currentVertex == s_MaxVerticesPerCall"
				//TODO draw all c urrent vertex in the currentBuffer
				CRenderableObjectTechnique* l_technique = m_Materials[currentSpriteMap->MaterialIndex]->GetRenderableObjectTechnique();
				m_Materials[currentSpriteMap->MaterialIndex]->Apply(l_technique);
				m_VertexBuffers[currentSpriteMap->MaterialIndex]->UpdateVertexs(m_CurrentBufferData, currentVertex);
				m_VertexBuffers[currentSpriteMap->MaterialIndex]->Render(RenderManager, l_technique->GetEffectTechnique(), &CEffectManager::m_SceneParameters, currentVertex);
			}
			currentVertex = 0;
			currentSpriteMap = commandSpriteMap;
		}
		int l_Height = RenderManager->GetContextManager()->GetHeight();
		int l_Width = RenderManager->GetContextManager()->GetWidth();
		float x1 = (command.x1 / (l_Height * 0.5f)) - 1.0f;
		float x2 = (command.x2 / (l_Width * 0.5f)) - 1.0f;
		float y1 = 1.0f - (command.y1 / (l_Height * 0.5f));
		float y2 = 1.0f - (command.y2 / (l_Width * 0.5f));

		float u1 = commandSprite->u1 * (1.0f - command.u1) + commandSprite->u2 * command.u1;
		float u2 = commandSprite->u1 * (1.0f - command.u2) + commandSprite->u2 * command.u2;
		float v1 = commandSprite->v1 * (1.0f - command.v1) + commandSprite->v2 * command.v1;
		float v2 = commandSprite->v1 * (1.0f - command.v2) + commandSprite->v2 * command.v2;

		m_CurrentBufferData[currentVertex++] = { Vect4f(x1, y2, 0.f, 1.f), command.color, Vect2f(u1, v2) };
		m_CurrentBufferData[currentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
		m_CurrentBufferData[currentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), command.color, Vect2f(u1, v1) };
	
		m_CurrentBufferData[currentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), command.color, Vect2f(u1, v1) };
		m_CurrentBufferData[currentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
		m_CurrentBufferData[currentVertex++] = { Vect4f(x2, y1, 0.f, 1.f), command.color, Vect2f(u2, v1) };
	}
	if (currentVertex > 0)
	{
		CRenderableObjectTechnique* l_technique = m_Materials[currentSpriteMap->MaterialIndex]->GetRenderableObjectTechnique();
		m_Materials[currentSpriteMap->MaterialIndex]->Apply(l_technique);
		m_VertexBuffers[currentSpriteMap->MaterialIndex]->UpdateVertexs(m_CurrentBufferData, currentVertex);
		m_VertexBuffers[currentSpriteMap->MaterialIndex]->Render(RenderManager, l_technique->GetEffectTechnique(), &CEffectManager::m_SceneParameters, currentVertex);
	}
	m_Commands.clear();
	m_InputUpToDate = false;
}