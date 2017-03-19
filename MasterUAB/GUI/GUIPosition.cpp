#include "GUIPosition.h"
#include "Engine\UABEngine.h"
#include "ContextManager\ContextManager.h"

CGUIPosition::CGUIPosition(float _x, float _y, float _w, float _h,
	CGUIManager::GUIAnchor _anchor,
	CGUIManager::GUICoordType _anchorCoordsType,
	CGUIManager::GUICoordType _sizeCorrdsType)
{
	int l_Width = UABEngine.GetRenderManager()->GetContextManager()->GetWidth();
	int l_Height = UABEngine.GetRenderManager()->GetContextManager()->GetHeight();


	switch (_sizeCorrdsType)
	{
	default:
		assert(false);
	case CGUIManager::GUICoordType::GUI_ABSOLUTE:
		m_width = (int)_w;
		m_height = (int)_h;
		break;
	case CGUIManager::GUICoordType::GUI_RELATIVE:

		m_width = (int)(_w * l_Width);
		m_height = (int)(_h * l_Height);
		break;
	case CGUIManager::GUICoordType::GUI_RELATIVE_WIDTH:
		m_width = (int)(_w * l_Width);
		m_height = (int)(_h * l_Width);
		break;
	case CGUIManager::GUICoordType::GUI_RELATIVE_HEIGHT:
		m_width = (int)(_w * l_Height);
		m_height = (int)(_h * l_Height);
		break;
	}

	float unitPixelSizeX, unitPixelSizeY;

	switch (_anchorCoordsType)
	{
	default:
		assert(false);
	case CGUIManager::GUICoordType::GUI_ABSOLUTE:
		unitPixelSizeX = 1;
		unitPixelSizeY = 1;
		break;
	case CGUIManager::GUICoordType::GUI_RELATIVE:
		unitPixelSizeX = (float)l_Width;
		unitPixelSizeY = (float)l_Height;
	case CGUIManager::GUICoordType::GUI_RELATIVE_WIDTH:
		unitPixelSizeX = (float)l_Width;
		unitPixelSizeY = (float)l_Width;
		break;
	case CGUIManager::GUICoordType::GUI_RELATIVE_HEIGHT:
		unitPixelSizeX = (float)l_Height;
		unitPixelSizeY = (float)l_Height;
		break;
	}
	float l_x, l_y;
	if (_x < 0)
	{
		l_x = 1.0f + _x;
	}
	else
	{
		l_x = _x;
	}

	if (_y < 0)
	{
		l_y = 1.0f + _y;
	}
	else
	{
		l_y = _y;
	}


	if ((int)_anchor & (int)CGUIManager::GUIAnchor::LEFT)
	{
		m_x = (int)(l_x * unitPixelSizeX);
	}
	else if ((int)_anchor & (int)CGUIManager::GUIAnchor::CENTER)
	{
		m_x = (int)(l_x * unitPixelSizeX - m_width * 0.5f);
	}
	else if ((int)_anchor & (int)CGUIManager::GUIAnchor::RIGHT)
	{
		m_x = (int)(l_x*unitPixelSizeX - m_width);
	}

	
	if ((int)_anchor & (int)CGUIManager::GUIAnchor::TOP)
	{
		m_y = (int)(_y * unitPixelSizeY);
	}
	else if ((int)_anchor & (int)CGUIManager::GUIAnchor::CENTER)
	{
		m_y = (int)(_y * unitPixelSizeY - m_height * 0.5f);
	}
	else if ((int)_anchor & (int)CGUIManager::GUIAnchor::BOTTOM)
	{
		m_y = (int)(_y*unitPixelSizeY - m_height);
	}

}

CGUIPosition::~CGUIPosition()
{
}