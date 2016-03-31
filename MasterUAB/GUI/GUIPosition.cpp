#include "GUIPosition.h"
#include "Engine\UABEngine.h"
#include "ContextManager\ContextManager.h"

CGUIPosition::CGUIPosition(float _x, float _y, float _w, float _h,
	GUIAnchor _anchor,
	GUICoordType _anchorCoordsType,
	GUICoordType _sizeCorrdsType)
{
	int l_Width = UABEngine.GetRenderManager()->GetContextManager()->GetWidth();
	int l_Height = UABEngine.GetRenderManager()->GetContextManager()->GetHeight();


	switch (_sizeCorrdsType)
	{
	default:
		assert(false);
	case GUICoordType::GUI_ABSOLUTE:
		m_width = _w;
		m_height = _h;
		break;
	case GUICoordType::GUI_RELATIVE:

		m_width = (int)(_w * l_Width);
		m_height = (int)(_h * l_Height);
		break;
	case GUICoordType::GUI_RELATIVE_WIDTH:
		m_width = (int)(_w * l_Width);
		m_height = (int)(_h * l_Width);
		break;
	case GUICoordType::GUI_RELATIVE_HEIGHT:
		m_width = (int)(_w * l_Height);
		m_height = (int)(_h * l_Height);
		break;
	}

	float unitPixelSizeX, unitPixelSizeY;

	switch (_anchorCoordsType)
	{
	default:
		assert(false);
	case GUICoordType::GUI_ABSOLUTE:
		unitPixelSizeX = 1;
		unitPixelSizeY = 1;
		break;
	case GUICoordType::GUI_RELATIVE:
		unitPixelSizeX = l_Width;
		unitPixelSizeY = l_Height;
	case GUICoordType::GUI_RELATIVE_WIDTH:
		unitPixelSizeX = l_Width;
		unitPixelSizeY = l_Width;
		break;
	case GUICoordType::GUI_RELATIVE_HEIGHT:
		unitPixelSizeX = l_Height;
		unitPixelSizeY = l_Height;
		break;
	}

	if (_x < 0)
	{
		_x = 1.0f + _x;
	}

	if ((int)_anchor & (int)GUIAnchor::LEFT)
	{
		m_x = (int)(_x * unitPixelSizeX);
	}
	else if ((int)_anchor & (int)GUIAnchor::CENTER)
	{
		m_x = (int)(_x * unitPixelSizeX - m_width * 0.5f);
	}
	else if ((int)_anchor & (int)GUIAnchor::RIGHT)
	{
		m_x = (int)(_x*unitPixelSizeX - m_width);
	}

	
	if ((int)_anchor & (int)GUIAnchor::TOP)
	{
		m_y = (int)(_y * unitPixelSizeY);
	}
	else if ((int)_anchor & (int)GUIAnchor::CENTER)
	{
		m_y = (int)(_y * unitPixelSizeY - m_height * 0.5f);
	}
	else if ((int)_anchor & (int)GUIAnchor::BOTTOM)
	{
		m_y = (int)(_y*unitPixelSizeY - m_height);
	}

}

CGUIPosition::~CGUIPosition()
{
}