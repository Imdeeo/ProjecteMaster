#include "Slider.h"
CSlider::CSlider(SpriteInfo* _Base, SpriteInfo* _Top, SpriteInfo* _Handle, SpriteInfo* PressedHandle)
{
	m_Base = _Base;
	m_Top = _Top;
	m_Handle = _Handle;
	m_PressedHandle = PressedHandle;
	float x1 = m_Handle->u1 * m_Handle->SpriteMap->w;
	float x2 = m_Handle->u2 * m_Handle->SpriteMap->w;
	float y1 = m_Handle->v1 * m_Handle->SpriteMap->h;
	float y2 = m_Handle->v2 * m_Handle->SpriteMap->h;

	handleRelativeWidth = (x2 - x1) / m_Handle->SpriteMap->w;
	handleRelativeHeight = (y2 - y1) / m_Handle->SpriteMap->h;
}

CSlider::~CSlider()
{
}
