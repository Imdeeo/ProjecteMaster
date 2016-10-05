#include "GUIManager.h"
#include "Materials\Material.h"
#include "Buton.h"
#include "Slider.h"
#include "Panel.h"
#include "RenderManager\RenderManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Materials\Material.h"
#include "Effects\EffectManager.h"
#include <assert.h>
#include "RenderableObjects\RenderableVertexs.h"
#include "InputManager\InputManager.h"
#include "ContextManager\ContextManager.h"
#include "XML\tinyxml2.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"
#include "GUIPosition.h"
#include "SliderResult.h"
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
	Destroy();
}

void CGUIManager::Destroy()
{
	for (size_t i = 0; i < m_Materials.size(); i++)
		CHECKED_DELETE(m_Materials[i]);
	m_Materials.clear();

	for (size_t i = 0; i < m_VertexBuffers.size(); i++)
		CHECKED_DELETE(m_VertexBuffers[i]);
	m_VertexBuffers.clear();

	for (auto it = m_Buttons.begin(); it != m_Buttons.end(); it++)
		CHECKED_DELETE(it->second);
	m_Buttons.clear();

	for (auto it = m_Sliders.begin(); it != m_Sliders.end(); it++)
		CHECKED_DELETE(it->second);
	m_Sliders.clear();

	for (auto it = m_TexturePerFont.begin(); it != m_TexturePerFont.end(); it++)
		it->second.clear();
	m_TexturePerFont.clear();

	m_LineHeightPerFont.clear();
	m_BasePerFont.clear();
	m_SpriteMaps.clear();
	m_Sprites.clear();
	m_Commands.clear();
	m_PanelCommands.clear();
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
		m_HotItem = id;
}

void CGUIManager::SetNotHot(const std::string& id)
{
	if (m_HotItem == id)
		m_HotItem = "";
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

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(_FileName.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;

	float u1, u2, v1, v2, w, h;
	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("gui_elements")->FirstChildElement();
		while (l_Element != NULL)
		{
			if (l_Element->Name() == std::string("gui_spritemap"))
			{
				std::string l_sprtieMapinfoname = l_Element->GetPszProperty("name");
				l_spriteMapinfo = { m_Materials.size(), l_Element->GetIntProperty("width"), l_Element->GetIntProperty("height") };
				m_SpriteMaps[l_sprtieMapinfoname] = l_spriteMapinfo;

				l_ElementAux = l_Element->FirstChildElement();
				while (l_ElementAux != NULL)
				{
					if (l_ElementAux->Name() == std::string("material"))
					{
						m_Materials.push_back(new CMaterial(l_ElementAux));
						m_VertexBuffers.push_back(new CUABTrianglesListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(m_CurrentBufferData, MAX_VERTICES_PER_CALL, MAX_VERTICES_PER_CALL / 3, true));
					}
					else if (l_ElementAux->Name() == std::string("sprite"))
					{
						w = (float)l_ElementAux->GetIntProperty("w");
						h = (float)l_ElementAux->GetIntProperty("h");
						u1 = (float)l_ElementAux->GetIntProperty("x");
						u2 = u1 + w;
						v1 = (float)l_ElementAux->GetIntProperty("y");
						v2 = v1 + h;
						l_sprite = { &m_SpriteMaps[l_sprtieMapinfoname], u1 / l_spriteMapinfo.w, u2 / l_spriteMapinfo.w, v1 / l_spriteMapinfo.h, v2 / l_spriteMapinfo.h };

						m_Sprites[l_ElementAux->GetPszProperty("name")] = l_sprite;
					}
					l_ElementAux = l_ElementAux->NextSiblingElement();
				}
			}
			else if (l_Element->Name() == std::string("button"))
				m_Buttons[l_Element->GetPszProperty("name")] = new CButon(&m_Sprites[l_Element->GetPszProperty("normal")], &m_Sprites[l_Element->GetPszProperty("highlight")], &m_Sprites[l_Element->GetPszProperty("pressed")]);
			else if (l_Element->Name() == std::string("panel"))
				m_Panels[l_Element->GetPszProperty("name")] = new CPanel(&m_Sprites[l_Element->GetPszProperty("normal")]);
			else if (l_Element->Name() == std::string("slider"))
				m_Sliders[l_Element->GetPszProperty("name")] = new CSlider(&m_Sprites[l_Element->GetPszProperty("base")], &m_Sprites[l_Element->GetPszProperty("top")], &m_Sprites[l_Element->GetPszProperty("handle")], &m_Sprites[l_Element->GetPszProperty("pressed_handle")]);
			else if (l_Element->Name() == std::string("font"))
			{
				std::string l_FontName = l_Element->GetPszProperty("name");
				std::string l_FileName = l_Element->GetPszProperty("path");

				tinyxml2::XMLDocument doc2;
				l_Error = doc2.LoadFile(l_FileName.c_str());
				if (l_Error == tinyxml2::XML_SUCCESS)
				{
					tinyxml2::XMLElement* l_Element2;
					tinyxml2::XMLElement* l_ElementAux2;
					tinyxml2::XMLElement* l_ElementAux3;
					l_Element2 = doc2.FirstChildElement("font");
					if (l_Element2!=NULL)
					{
						l_ElementAux2 = l_Element2->FirstChildElement();
						while (l_ElementAux2 != NULL)
						{
							if (l_ElementAux2->Name() == std::string("common"))
							{
								m_LineHeightPerFont[l_FontName] = l_ElementAux2->GetIntProperty("lineHeight");
								m_BasePerFont[l_FontName] = l_ElementAux2->GetIntProperty("base");
							}
							else if (l_ElementAux2->Name() == std::string("pages"))
							{
								l_ElementAux3 = l_ElementAux2->FirstChildElement();
								while (l_ElementAux3!=NULL)
								{
									if (l_ElementAux3->Name() == std::string("page"))
										m_TexturePerFont[l_FontName].push_back(&m_Sprites[l_ElementAux3->GetPszProperty("file")]);
									l_ElementAux3 = l_ElementAux3->NextSiblingElement();
								}
							}
							else if (l_ElementAux2->Name() == std::string("chars"))
							{
								l_ElementAux3 = l_ElementAux2->FirstChildElement();
								while (l_ElementAux3 != NULL)
								{
									if (l_ElementAux3->Name() == std::string("char"))
									{
										FontChar l_FontChar = { l_ElementAux3->GetIntProperty("x"), l_ElementAux3->GetIntProperty("y"), l_ElementAux3->GetIntProperty("width"), l_ElementAux3->GetIntProperty("height"),
											l_ElementAux3->GetIntProperty("xoffset"), l_ElementAux3->GetIntProperty("yoffset"), l_ElementAux3->GetIntProperty("xadvance"), l_ElementAux3->GetIntProperty("page"), l_ElementAux3->GetIntProperty("chnl") };
										m_CharactersPerFont[l_FontName][l_ElementAux3->GetIntProperty("id")] = l_FontChar;
									}
									l_ElementAux3 = l_ElementAux3->NextSiblingElement();
								}
							}
							else if (l_ElementAux2->Name() == std::string("kernings"))
							{
								l_ElementAux3 = l_ElementAux2->FirstChildElement();
								while (l_ElementAux3 != NULL)
								{
									if (l_ElementAux3->Name() == std::string("kerning"))
										m_KerningsPerFont[l_FontName][l_ElementAux3->GetIntProperty("first")][l_ElementAux3->GetIntProperty("second")] = l_ElementAux3->GetIntProperty("second");
									l_ElementAux3 = l_ElementAux3->NextSiblingElement();
								}
							}
							l_ElementAux2 = l_ElementAux2->NextSiblingElement();
						}

					}
				}
			}
			l_Element = l_Element->NextSiblingElement();
		}
	}
	else
		return false;
	return true;
}

void CGUIManager::CheckInput()
{
	if (!m_InputUpToDate)
	{
		gainput::InputMap* l_InputMap = UABEngine.GetInputManager()->GetMap();
		m_MouseX = (int)(WINDOW_WIDTH*l_InputMap->GetFloat(CInputManager::Actions::AxisX));
		m_MouseY = (int)(WINDOW_HEIGHT*l_InputMap->GetFloat(CInputManager::Actions::AxisY));

		m_MouseWentReleased = l_InputMap->GetBoolWasDown(CInputManager::Actions::LeftClick);
		m_MouseWentPressed = l_InputMap->GetBool(CInputManager::Actions::LeftClick);
		m_InputUpToDate = true;
	}
}

bool IsMouseInside(int _mouseX, int _mouseY, int x, int y, int width, int height)
{
	if ((_mouseX >= x && _mouseX <= x + width) && (_mouseY >= y && _mouseY <= y + height))
			return true;
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
				l_result = true;
			SetNotActive();
		}	
	}
	else if (m_HotItem == guiID)
	{
		if (m_MouseWentPressed)
			SetActive(guiID);
	}

	if (IsMouseInside(m_MouseX, m_MouseY, position.Getx(), position.Gety(), position.Getwidth(), position.Getheight()))
		SetHot(guiID);
	else
		SetNotHot(guiID);

	if (m_ActiveItem == guiID && m_HotItem == guiID)
		l_sprite = m_Buttons[buttonID]->GetPressed();
	else
	{
		if (m_HotItem == guiID)
			l_sprite = m_Buttons[buttonID]->GetHighlight();
		else
			l_sprite = m_Buttons[buttonID]->GetNormal();
	}

	GUICommand command = { 
		l_sprite,
		position.Getx(), position.Gety(), position.Getx() + position.Getwidth(), position.Gety() + position.Getheight(),
		0, 0, 1, 1,
		CColor(1, 1, 1, 1) };
	m_Commands.push_back(command);
	
	return l_result;
}

void CGUIManager::DoPanel(const std::string& guiID, const std::string& panelID, const CGUIPosition& position)
{
	SpriteInfo* l_sprite = m_Panels[panelID]->GetNormal();
	CheckInput();

	GUICommand command = {
		l_sprite,position.Getx(), position.Gety(), position.Getx() + position.Getwidth(), position.Gety() + position.Getheight(),
		0, 0, 1, 1,
		CColor(1, 1, 1, 1) };
	m_PanelCommands.push_back(command);

	//return l_result;
}


CSliderResult CGUIManager::DoSlider(const std::string& GuiID, const std::string& SliderID, const CGUIPosition& Position, float MinValue, float MaxValue, float CurrentValue, bool _Interactuable)
{
	CSlider* l_Slider = m_Sliders[SliderID];
	CSliderResult l_Result = CSliderResult(.0f, .0f);

	if (l_Slider != nullptr)
	{
		CheckInput();
		if (_Interactuable)
		{
			bool RealResult = false;

			float l_Factor = (float)(m_MouseX - Position.Getx()) / ((float)Position.Getwidth());
			if (l_Factor < 0) l_Factor = 0;
			else if (l_Factor > 1) l_Factor = 1;

			l_Result.m_Temp = MinValue + (MaxValue - MinValue)*l_Factor;

			if (m_ActiveItem == GuiID)
			{
				if (m_MouseWentReleased)
				{
					if (m_HotItem == GuiID)
						RealResult = true;
					SetNotActive();
				}
			}
			else if (m_HotItem == GuiID)
			{
				if (m_MouseWentPressed)
					SetActive(GuiID);
			}

			if (RealResult)
				l_Result.m_Real = l_Result.m_Temp;
			else if (m_ActiveItem == GuiID)
				l_Result.m_Real = CurrentValue;
			else
			{
				l_Result.m_Temp = CurrentValue;
				l_Result.m_Real = CurrentValue;
			}

			float l_HandlePosition = Position.Getx() + Position.Getwidth() * (l_Result.m_Temp - MinValue) / (MaxValue - MinValue);
			float l_RealHandleWidth = l_Slider->handleRelativeWidth * Position.Getwidth();
			float l_RealHandleHeight = l_Slider->handleRelativeHeight * Position.Getheight();

			int l_RealHandleX = (int)(l_HandlePosition - l_RealHandleWidth * 0.5f);
			int l_RealHandleY = (int)(Position.Gety() + Position.Getheight() * 0.5f - l_RealHandleHeight * 0.5);

			if (IsMouseInside(m_MouseX, m_MouseY, Position.Getx(), Position.Gety(), Position.Getwidth(), Position.Getheight()))
				SetHot(GuiID);
			else if (IsMouseInside(m_MouseX, m_MouseY, l_RealHandleX, l_RealHandleY, (int)l_RealHandleWidth, (int)l_RealHandleHeight))
				SetHot(GuiID);
			else
				SetNotHot(GuiID);

			GUICommand l_Base = { l_Slider->GetBase(), (int)Position.Getx(), (int)Position.Gety(), (int)(Position.Getx() + Position.Getwidth()), (int)(Position.Gety() + Position.Getheight())
				, 0, 0, 1, 1,
				CColor(1.0f, 1.0f, 1.0f, 1.0f) };
			m_Commands.push_back(l_Base);

			GUICommand l_Top = { l_Slider->GetTop(), (int)Position.Getx(), (int)Position.Gety(), (int)l_HandlePosition, int(Position.Gety() + Position.Getheight()),
				0, 0, (l_Result.m_Temp - MinValue) / (MaxValue - MinValue), 1,
				CColor(1.0f, 1.0f, 1.0f, 1.0f) };
			m_Commands.push_back(l_Top);

			GUICommand l_Handle = {
				(m_ActiveItem == GuiID && m_HotItem == GuiID) ? l_Slider->GetPressedHandle() : l_Slider->GetHandle(),
				l_RealHandleX, l_RealHandleY, l_RealHandleX + (int)l_RealHandleWidth, l_RealHandleY + (int)l_RealHandleHeight,
				0, 0, 1, 1,
				CColor(1.0f, 1.0f, 1.0f, 1.0f) };
			m_Commands.push_back(l_Handle);
		}
		else
		{
			float l_HandlePosition = Position.Getx() + Position.Getwidth() * (CurrentValue - MinValue) / (MaxValue - MinValue);
			GUICommand l_Base = { l_Slider->GetBase(), (int)Position.Getx(), (int)Position.Gety(), (int)(Position.Getx() + Position.Getwidth()), (int)(Position.Gety() + Position.Getheight())
				, 0, 0, 1, 1,
				CColor(1.0f, 1.0f, 1.0f, 1.0f) };
			m_Commands.push_back(l_Base);

			GUICommand l_Top = { l_Slider->GetTop(), (int)Position.Getx(), (int)Position.Gety(), (int)l_HandlePosition, int(Position.Gety() + Position.Getheight()),
				0, 0, (CurrentValue - MinValue) / (MaxValue - MinValue), 1,
				CColor(1.0f, 1.0f, 1.0f, 1.0f) };
			m_Commands.push_back(l_Top);
		}
	
	}

	return l_Result;
}

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

	float spritewidth = (float)l_TextureArray[0]->SpriteMap->w;
	float spriteHeight = (float)l_TextureArray[0]->SpriteMap->h;

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

				if (command.x1 < textBox_->x) textBox_->x = (float)command.x1;
				if (command.y1 < textBox_->y) textBox_->y = (float)command.y1;
				if (command.x2 < textBox_->z) textBox_->z = (float)command.x2;
				if (command.y2 < textBox_->w) textBox_->w = (float)command.y2;
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
		adjustment.y -= textSizes.y;
	else if ((int)_anchor & (int)GUIAnchor::MID)
		adjustment.y -= (textSizes.y + textSizes.w) * 0.5f;
	else if ((int)_anchor & (int)GUIAnchor::BOTTOM)
		adjustment.y -= textSizes.w;
	else
		assert(false);

	for (size_t i = m_Commands.size() - numCommands; i < m_Commands.size(); ++i)
	{
		m_Commands[i].x1 += (int)adjustment.x;
		m_Commands[i].x2 += (int)adjustment.x;
		m_Commands[i].y1 += (int)adjustment.y;
		m_Commands[i].y2 += (int)adjustment.y;
	}
}

std::string CGUIManager::DoTextBox(const std::string& guiID, const std::string& _font, const std::string& currentText, CGUIPosition position)
{
	CheckInput();
	
	if (IsMouseInside(m_MouseX, m_MouseY, position.Getx(), position.Gety(), position.Getwidth(), position.Getheight()))
		SetHot(guiID);
	else
		SetNotHot(guiID);

	if (m_ActiveItem == guiID )
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
	else if (m_HotItem == guiID)
	{
		if (m_MouseWentPressed)
			SetActive(guiID);
	}

	std::string displayText;
	std::string activeText = currentText;

	if (guiID == m_SelectedItem) 
	{
		CKeyboardInput* l_KeyBoard = UABEngine.GetInputManager()->GetKeyBoard();
		if (l_KeyBoard->isConsumible())
		{
			wchar_t lastChar = l_KeyBoard->ConsumeLastChar();
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
				if (activeText.length() > 2)
					activeText = activeText.substr(0, activeText.length() - 1);
			}
		}		
	}

	FillCommandQueueWithText(_font, activeText, Vect2f(position.Getx() + position.Getwidth() * 0.05f,
		position.Gety() + position.Getheight() * 0.75f));
	return activeText;
}


void CGUIManager::Render(CRenderManager *RenderManager)
{
	int currentVertex = 0;
	SpriteMapInfo *currentSpriteMap = nullptr;
	for (size_t i = 0; i < m_PanelCommands.size(); ++i)  //commandsExecutionOrder.size()
	{
		GUICommand &command = m_PanelCommands[i];
		assert(command.x1 <= command.x2);
		assert(command.y2 <= command.y2);

		SpriteInfo *commandSprite = command.sprite;
		SpriteMapInfo *commandSpriteMap = commandSprite->SpriteMap;

		if (currentSpriteMap != commandSpriteMap || currentVertex + 6 >= MAX_VERTICES_PER_CALL)
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

		assert(MAX_VERTICES_PER_CALL > currentVertex + 6);
		m_CurrentBufferData[currentVertex++] = { Vect4f(x1, y2, 0.f, 1.f), command.color, Vect2f(u1, v2) };
		m_CurrentBufferData[currentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
		m_CurrentBufferData[currentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), command.color, Vect2f(u1, v1) };
	
		m_CurrentBufferData[currentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), command.color, Vect2f(u1, v1) };
		m_CurrentBufferData[currentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
		m_CurrentBufferData[currentVertex++] = { Vect4f(x2, y1, 0.f, 1.f), command.color, Vect2f(u2, v1) };
	}

	for (size_t i = 0; i < m_Commands.size(); ++i)  //commandsExecutionOrder.size()
	{
		GUICommand &command = m_Commands[i];
		assert(command.x1 <= command.x2);
		assert(command.y2 <= command.y2);

		SpriteInfo *commandSprite = command.sprite;
		SpriteMapInfo *commandSpriteMap = commandSprite->SpriteMap;

		if (currentSpriteMap != commandSpriteMap || currentVertex + 6 >= MAX_VERTICES_PER_CALL)
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

		assert(MAX_VERTICES_PER_CALL > currentVertex + 6);
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
	m_PanelCommands.clear();
	m_InputUpToDate = false;
}

