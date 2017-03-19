#include "Buton.h"
#include "Engine\UABEngine.h"
#include "GUIManager.h"


CButon::CButon(SpriteInfo* _normal, SpriteInfo* _highlight, SpriteInfo* _pressed)
{
	m_Normal = _normal;
	m_Highlight = _highlight;
	m_Pressed = _pressed;
}

CButon::~CButon()
{
}