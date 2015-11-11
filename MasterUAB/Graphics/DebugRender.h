#pragma once

#include <d3d11.h>

#include "RenderableVertexs.h"
//#include "Effects.hpp"

class CEffect;

class CDebugRender
{
public:
	CDebugRender(ID3D11Device* _Device);


	CRenderableVertexs * GetSimpleTriangle() const { return m_SimpleTriangle; }
	CRenderableVertexs * GetSimpleCube() const { return m_SimpleCube; }

private:

	CRenderableVertexs				*m_SimpleTriangle;
	CRenderableVertexs				*m_SimpleGrid;
	CRenderableVertexs				*m_SimpleCube;
	CRenderableVertexs				*m_AxisRenderableVertexs;
	CRenderableVertexs				*m_LongAxisRenderableVertexs;
};

