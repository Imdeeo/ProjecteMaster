#pragma once

#include <d3d11.h>

#include "RenderableVertexs.h"
#include "Math\Vector3.h"

class CEffect;

class CDebugRender
{
public:
	CDebugRender(ID3D11Device* _Device);


	CRenderableVertexs * GetSimpleTriangle() const { return m_SimpleTriangle; }
	CRenderableVertexs * GetClassicBlendTriangle() const { return m_ClassicBlendTriangle; }
	CRenderableVertexs * GetPremultBlendTriangle() const { return m_PremultBlendTriangle; }
	CRenderableVertexs * GetSimpleCube() const { return m_SimpleCube; }
	CRenderableVertexs * GetAxis() const { return m_Axis; }

	float GetSimpleTriangleBSRadi() const { return m_SimpleTriangleBSRadi; }
	float GetClassicBlendTriangleBSRadi() const { return m_ClassicBlendTriangleBSRadi; }
	float GetPremultBlendTriangleBSRadi() const { return m_PremultBlendTriangleBSRadi; }
	float GetSimpleCubeBSRadi() const { return m_SimpleCubeBSRadi; }
	float GetAxisBSRadi() const { return m_AxisBSRadi; }

	Vect3f GetSimpleTriangleBBMin() const { return m_SimpleTriangleBBMin; }
	Vect3f GetClassicBlendTriangleBBMin() const { return m_ClassicBlendTriangleBBMin; }
	Vect3f GetPremultBlendTriangleBBMin() const { return m_PremultBlendTriangleBBMin; }
	Vect3f GetSimpleCubeBBMin() const { return m_SimpleCubeBBMin; }
	Vect3f GetAxisBBMin() const { return m_AxisBBMin; }

	Vect3f GetSimpleTriangleBBMax() const { return m_SimpleTriangleBBMax; }
	Vect3f GetClassicBlendTriangleBBMax() const { return m_ClassicBlendTriangleBBMax; }
	Vect3f GetPremultBlendTriangleBBMax() const { return m_PremultBlendTriangleBBMax; }
	Vect3f GetSimpleCubeBBMax() const { return m_SimpleCubeBBMax; }
	Vect3f GetAxisBBMax() const { return m_AxisBBMax; }

private:

	CRenderableVertexs				*m_SimpleTriangle;
	CRenderableVertexs				*m_ClassicBlendTriangle;
	CRenderableVertexs				*m_PremultBlendTriangle;
	CRenderableVertexs				*m_SimpleGrid;
	CRenderableVertexs				*m_SimpleCube;
	CRenderableVertexs				*m_Axis;
	CRenderableVertexs				*m_LongAxis;

	float m_SimpleTriangleBSRadi;
	float m_ClassicBlendTriangleBSRadi;
	float m_PremultBlendTriangleBSRadi;
	float m_SimpleGridBSRadi;
	float m_SimpleCubeBSRadi;
	float m_AxisBSRadi;
	float m_LongAxisBSRadi;

	Vect3f m_SimpleTriangleBBMin;
	Vect3f m_ClassicBlendTriangleBBMin;
	Vect3f m_PremultBlendTriangleBBMin;
	Vect3f m_SimpleGridBBMin;
	Vect3f m_SimpleCubeBBMin;
	Vect3f m_AxisBBMin;
	Vect3f m_LongAxisBBMin;

	Vect3f m_SimpleTriangleBBMax;
	Vect3f m_ClassicBlendTriangleBBMax;
	Vect3f m_PremultBlendTriangleBBMax;
	Vect3f m_SimpleGridBBMax;
	Vect3f m_SimpleCubeBBMax;
	Vect3f m_AxisBBMax;
	Vect3f m_LongAxisBBMax;
};

