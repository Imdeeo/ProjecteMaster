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
#include "GUIPosition.h"
#include <assert.h>


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
	m_SelectedItem = "";
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

void CGUIManager::SetSelected(const std::string& id)
{
	m_SelectedItem = id;
}

void CGUIManager::SetNotSelected(const std::string& id)
{
	if (m_SelectedItem == id)
		m_SelectedItem = "";
}

bool CGUIManager::Load(std::string _FileName)
{
	m_FileName = _FileName;
	SpriteMapInfo l_spriteMapinfo;
	SpriteInfo l_sprite;
	CXMLTreeNode l_XML;
	float u1, u2, v1, v2, w, h;
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
					for (int j = 0; j < l_Element.GetNumChildren(); ++j)
					{
						CXMLTreeNode l_aux = l_Element(j);
						if (l_aux.GetName() == std::string("material"))
						{
							m_Materials.push_back(new CMaterial(l_aux));
							m_VertexBuffers.push_back(new CUABTrianglesListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(m_CurrentBufferData, MAX_VERTICES_PER_CALL, MAX_VERTICES_PER_CALL/3, true));
						}
						else if (l_aux.GetName() == std::string("sprite"))
						{
							w = l_aux.GetIntProperty("w");
							h = l_aux.GetIntProperty("h");
							u1 = l_aux.GetIntProperty("x");
							u2 = u1 + w;
							v1 = l_aux.GetIntProperty("y");
							v2 = v1 + h;
							l_sprite = { &m_SpriteMaps[l_sprtieMapinfoname], u1 / l_spriteMapinfo.w, u2 / l_spriteMapinfo.w, v1 / l_spriteMapinfo.h, v2 / l_spriteMapinfo.h };
						
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
				else if (l_Element.GetName() == std::string("font"))
				{
					CXMLTreeNode l_XMLfont;
					std::string l_FontName = l_Element.GetPszProperty("name");
					std::string l_FileName = l_Element.GetPszProperty("path");
					if (l_XMLfont.LoadFile(l_FileName.c_str()))
					{
						CXMLTreeNode l_InputFile = l_XMLfont["font"];
						if (l_InputFile.Exists())
						{
							for (int k = 0; k < l_InputFile.GetNumChildren(); ++k)
							{
								CXMLTreeNode l_ElementFile = l_InputFile(k);
								if (l_ElementFile.GetName() == std::string("common"))
								{
									m_LineHeightPerFont[l_FontName] = l_ElementFile.GetIntProperty("lineHeight");
									m_BasePerFont[l_FontName] = l_ElementFile.GetIntProperty("base");
								}
								else if (l_ElementFile.GetName() == std::string("pages"))
								{
									for (int l = 0; l < l_ElementFile.GetNumChildren(); ++l)
									{
										CXMLTreeNode l_PagesFile = l_ElementFile(l);
										if (l_PagesFile.GetName() == std::string("page"))
										{
											m_TexturePerFont[l_FontName].push_back(&m_Sprites[l_PagesFile.GetPszProperty("file")]);
										}										
									}
								}
								else if (l_ElementFile.GetName() == std::string("chars"))
								{
									for (int l = 0; l < l_ElementFile.GetNumChildren(); ++l)
									{
										CXMLTreeNode l_CharFile = l_ElementFile(l);
										if (l_CharFile.GetName() == std::string("char"))
										{
											FontChar l_FontChar = { l_CharFile.GetIntProperty("x"), l_CharFile.GetIntProperty("y"), l_CharFile.GetIntProperty("width"), l_CharFile.GetIntProperty("height"),
												l_CharFile.GetIntProperty("xoffset"), l_CharFile.GetIntProperty("yoffset"), l_CharFile.GetIntProperty("xadvance"), l_CharFile.GetIntProperty("page"), l_CharFile.GetIntProperty("chnl") };
											m_CharactersPerFont[l_FontName][l_CharFile.GetIntProperty("id")] = l_FontChar;
										}
									}
								}
								else if (l_ElementFile.GetName() == std::string("kernings"))
								{
									for (int l = 0; l < l_ElementFile.GetNumChildren(); ++l)
									{
										CXMLTreeNode l_KerningFile = l_ElementFile(l);
										if (l_KerningFile.GetName() == std::string("kerning"))
										{
											m_KerningsPerFont[l_FontName][l_KerningFile.GetIntProperty("first")][l_KerningFile.GetIntProperty("second")] = l_KerningFile.GetIntProperty("second");
										}
									}
								}
							}

						}
					}
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

		if (m_MouseWentPressed && !l_InputManager->IsActionActive("MOUSE_CLICK_LEFT"))
		{
			m_MouseWentReleased = true;
		}
		else
		{
			m_MouseWentReleased = false;
		}
		m_MouseWentPressed = l_InputManager->IsActionActive("MOUSE_CLICK_LEFT");
		

		m_InputUpToDate = true;
	}
}

bool IsMouseInside(int _mouseX, int _mouseY, int x, int y, int width, int height)
{
	if (_mouseX >= x && _mouseX <= x + width)
	{
		if (_mouseY >= y && _mouseY <= y + height)
		{
			return true;
		}
	}
	return false;
}

bool CGUIManager::DoButton(const std::string& guiID, const std::string& buttonID, const CGUIPosition& position)
{
	SpriteInfo* l_sprite;
	CheckInput();
	bool l_result = false;

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
		}
	}

	if (IsMouseInside(m_MouseX, m_MouseY, position.Getx(), position.Gety(), position.Getwidth(), position.Getheight()))
	{
		SetHot(guiID);		
	}
	else
	{
		SetNotHot(guiID);
	}

	if (m_ActiveItem == guiID && m_HotItem == guiID)
	{
		l_sprite = m_Buttons[buttonID]->GetPressed();
	}
	else
	{
		if (m_HotItem == guiID)
		{
			l_sprite = m_Buttons[buttonID]->GetHighlight();
		}
		else
		{
			l_sprite = m_Buttons[buttonID]->GetNormal();
		}
	}

	GUICommand command = { 
		l_sprite,
		position.Getx(), position.Gety(), position.Getx() + position.Getwidth(), position.Gety() + position.Getheight(),
		0, 0, 1, 1,
		CColor(1, 1, 1, 1) };
	m_Commands.push_back(command);
	
	return l_result;
}



/*SliderResult CGUIManager::DoSlider(const std::string& guiID, const std::string& sliderID, const GUIPosition& position, float minValue, float maxValue, float currentValue)
{

}*/

int CGUIManager::FillCommandQueueWithTextAux(const std::string& _font, const std::string& _text, const CColor& _color, Vect4f *textBox_)
{
	Vect4f dummy;
	if (textBox_ == nullptr) textBox_ = &dummy;

	*textBox_ = Vect4f(0, 0, 0, 0);

	assert(m_LineHeightPerFont.find(_font) != m_LineHeightPerFont.end());
	assert(m_BasePerFont.find(_font) != m_BasePerFont.end());
	assert(m_CharactersPerFont.find(_font) != m_CharactersPerFont.end());
	assert(m_KerningsPerFont.find(_font) != m_KerningsPerFont.end());
	assert(m_TexturePerFont.find(_font) != m_TexturePerFont.end());
	
	int lineHeight = m_LineHeightPerFont[_font];
	int base = m_BasePerFont[_font];
	const std::unordered_map< wchar_t, FontChar > &l_CharacterMap = m_CharactersPerFont[_font];
	const std::unordered_map< wchar_t, std::unordered_map< wchar_t, int>> &l_Kernings = m_KerningsPerFont[_font];
	const std::vector<SpriteInfo*> &l_TextureArray = m_TexturePerFont[_font];

	wchar_t last = 0;

	int cursorX = 0, cursorY = 0;

	float spritewidth = l_TextureArray[0]->SpriteMap->w;
	float spriteHeight = l_TextureArray[0]->SpriteMap->h;

	int addedCommands = 0;

	for (char c : _text)
	{
		if (c == '\n')
		{
			cursorY += lineHeight;
			cursorX = 0;
			last = 0;
		}
		else
		{
			auto it = l_CharacterMap.find((wchar_t)c);
			if (it != l_CharacterMap.end())
			{
				const FontChar &fontChar = it->second;
				auto it1 = l_Kernings.find(last);
				if (it1 != l_Kernings.end())
				{
					auto it2 = it1->second.find(c);
					if (it2 != it1->second.end())
					{
						int kerning = it2->second;
						cursorX += kerning;
					}
				}
				GUICommand command = {};
				command.sprite = l_TextureArray[fontChar.page];
				command.x1 = cursorX + fontChar.xoffset;
				command.x2 = command.x1 + fontChar.width;
				command.y1 = cursorY - base + fontChar.yoffset;
				command.y2 = command.y1 + fontChar.height;

				command.u1 = (float)fontChar.x / spritewidth;
				command.u2 = (float)(fontChar.x + fontChar.width) / spritewidth;
				command.v1 = (float)fontChar.y / spriteHeight;
				command.v2 = (float)(fontChar.y + fontChar.height) / spriteHeight;

				command.color = _color;

				m_Commands.push_back(command);
				++addedCommands;

				last = c;
				cursorX += fontChar.xadvance;

				if (command.x1 < textBox_->x) textBox_->x = command.x1;
				if (command.y1 < textBox_->y) textBox_->y = command.y1;
				if (command.x2 < textBox_->z) textBox_->z = command.x2;
				if (command.y2 < textBox_->w) textBox_->w = command.y2;
			}
		}
	}

	return addedCommands;
}

void CGUIManager::FillCommandQueueWithText(const std::string& _font, const std::string& _text,
	Vect2f _coord, GUIAnchor _anchor, const CColor& _color)
{
	Vect4f textSizes;
	
	int numCommands = FillCommandQueueWithTextAux(_font, _text, _color, &textSizes);

	Vect2f adjustment = _coord;

	if ((int)_anchor & (int)GUIAnchor::TOP)
	{
		adjustment.y -= textSizes.y;
	}
	else if ((int)_anchor & (int)GUIAnchor::MID)
	{
		adjustment.y -= (textSizes.y + textSizes.w) * 0.5f;
	}
	else if ((int)_anchor & (int)GUIAnchor::BOTTOM)
	{
		adjustment.y -= textSizes.w;
	}
	else
	{
		assert(false);
	}

	for (int i = m_Commands.size() - numCommands; i < m_Commands.size(); ++i)
	{
		m_Commands[i].x1 += adjustment.x;
		m_Commands[i].x2 += adjustment.x;
		m_Commands[i].y1 += adjustment.y;
		m_Commands[i].y2 += adjustment.y;
	}
}

std::string CGUIManager::DoTextBox(const std::string& guiID, const std::string& _font, const std::string& currentText, CGUIPosition position)
{
	if (m_ActiveItem == guiID)
	{
		if (m_MouseWentReleased)
		{
			if (m_HotItem == guiID)
			{
				SetSelected(guiID);
			}
			SetNotActive();
		}
	}

	std::string displayText;
	std::string activeText = currentText;

	if (guiID == m_SelectedItem)
	{
		CKeyboardInput* keyboard = CInputManager::GetInputManager()->GetKeyboard();
		wchar_t lastChar = keyboard->ConsumeLastChar();
		if (lastChar >= 0x20 && lastChar < 255)
		{
			activeText += (char)lastChar;
		}
		else if (lastChar == '\r')
		{
			activeText += '\n';
		}
		else if (lastChar == '\b')
		{
			activeText = activeText.substr(0, activeText.length() - 1);
		}
	}

	FillCommandQueueWithText(_font, displayText, Vect2f(position.Getx() + position.Getwidth() * 0.05f,
		position.Gety() + position.Getheight() * 0.75f));
	return activeText;
}


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
		float x1 = (command.x1 / (l_Width * 0.5f)) - 1.0f;
		float x2 = (command.x2 / (l_Width * 0.5f)) - 1.0f;
		float y1 = 1.0f - (command.y1 / (l_Height * 0.5f));
		float y2 = 1.0f - (command.y2 / (l_Height * 0.5f));

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

