#ifdef _DEBUG

#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

/*class CAnimatedModelCore
{
public:
	

private:

};

#endif*/



#include "Math\Vector3.h"
#include "Utils.h"

class CEffect;
class ID3D11Device;
class CRenderableObjectTechnique;
class CRenderableVertexs;
class CEffectTechnique;

class CDebugRender
{
public:
	CDebugRender(ID3D11Device* _Device);
	~CDebugRender();

	CRenderableVertexs * GetLine(Vect3f inici, Vect3f final) const;

	CRenderableVertexs * GetSimpleTriangle() const { return m_SimpleTriangle; }
	CRenderableVertexs * GetClassicBlendTriangle() const { return m_ClassicBlendTriangle; }
	CRenderableVertexs * GetPremultBlendTriangle() const { return m_PremultBlendTriangle; }
	CRenderableVertexs * GetSimpleCube() const { return m_SimpleCube; }
	CRenderableVertexs * GetAxis() const { return m_Axis; }
	CRenderableVertexs * GetSimleGrid() const { return m_SimpleGrid; }
	CRenderableVertexs * GetBigGrid() const { return m_BigGrid; }

	CRenderableVertexs * GetQuadRV() const { return m_DrawQuadRV; }

	CRenderableVertexs * GetSPhere10() const { return m_Sphere_10Seg; }
	CRenderableVertexs * GetSPhere10_2() const { return m_Sphere_10Seg2; }
	CRenderableVertexs * GetCone() const { return m_Cone; }

	float GetSimpleTriangleBSRadi() const { return m_SimpleTriangleBSRadi; }
	float GetClassicBlendTriangleBSRadi() const { return m_ClassicBlendTriangleBSRadi; }
	float GetPremultBlendTriangleBSRadi() const { return m_PremultBlendTriangleBSRadi; }
	float GetSimpleCubeBSRadi() const { return m_SimpleCubeBSRadi; }
	float GetAxisBSRadi() const { return m_AxisBSRadi; }
	float GetSphere10BSRadi() const { return m_Sphere_10SegBSRadi; }
	float GetConeBSRadi() const { return m_ConeBSRadi; }

	Vect3f GetSimpleTriangleBBMin() const { return m_SimpleTriangleBBMin; }
	Vect3f GetClassicBlendTriangleBBMin() const { return m_ClassicBlendTriangleBBMin; }
	Vect3f GetPremultBlendTriangleBBMin() const { return m_PremultBlendTriangleBBMin; }
	Vect3f GetSimpleCubeBBMin() const { return m_SimpleCubeBBMin; }
	Vect3f GetAxisBBMin() const { return m_AxisBBMin; }
	Vect3f GetSPhere10BBMin() const { return m_Sphere_10SegBBMin; }
	Vect3f GetConeBBMin() const { return m_ConeBBMin; }

	Vect3f GetSimpleTriangleBBMax() const { return m_SimpleTriangleBBMax; }
	Vect3f GetClassicBlendTriangleBBMax() const { return m_ClassicBlendTriangleBBMax; }
	Vect3f GetPremultBlendTriangleBBMax() const { return m_PremultBlendTriangleBBMax; }
	Vect3f GetSimpleCubeBBMax() const { return m_SimpleCubeBBMax; }
	Vect3f GetAxisBBMax() const { return m_AxisBBMax; }
	Vect3f GetSphere10BBMax() const { return m_Sphere_10SegBBMax; }
	Vect3f GetConeBBMax() const { return m_ConeBBMax; }

	Vect3f GetSphere10BSCenter() const { return m_Sphere_10SegBSCenter; }
	Vect3f GetConeBSCenter() const { return m_ConeBSCenter; }

	CEffectTechnique *	GetDebugTechnique() const;

private:
	//CRenderableObjectTechnique		*m_EffectTechnique;

	CRenderableVertexs				*m_SimpleTriangle;
	CRenderableVertexs				*m_ClassicBlendTriangle;
	CRenderableVertexs				*m_PremultBlendTriangle;
	CRenderableVertexs				*m_SimpleGrid;
	CRenderableVertexs				*m_BigGrid;
	CRenderableVertexs				*m_SimpleCube;
	CRenderableVertexs				*m_Axis;
	CRenderableVertexs				*m_LongAxis;
	CRenderableVertexs				*m_DrawQuadRV;

	CRenderableVertexs				*m_Sphere_10Seg;
	CRenderableVertexs				*m_Sphere_10Seg2;
	CRenderableVertexs				*m_Cone;

	float m_SimpleTriangleBSRadi;
	float m_ClassicBlendTriangleBSRadi;
	float m_PremultBlendTriangleBSRadi;
	float m_SimpleGridBSRadi;
	float m_SimpleCubeBSRadi;
	float m_AxisBSRadi;
	float m_LongAxisBSRadi;
	float m_Sphere_10SegBSRadi;
	float m_ConeBSRadi;

	Vect3f m_SimpleTriangleBBMin;
	Vect3f m_ClassicBlendTriangleBBMin;
	Vect3f m_PremultBlendTriangleBBMin;
	Vect3f m_SimpleGridBBMin;
	Vect3f m_SimpleCubeBBMin;
	Vect3f m_AxisBBMin;
	Vect3f m_LongAxisBBMin;
	Vect3f m_Sphere_10SegBBMin;
	Vect3f m_ConeBBMin;

	Vect3f m_SimpleTriangleBBMax;
	Vect3f m_ClassicBlendTriangleBBMax;
	Vect3f m_PremultBlendTriangleBBMax;
	Vect3f m_SimpleGridBBMax;
	Vect3f m_SimpleCubeBBMax;
	Vect3f m_AxisBBMax;
	Vect3f m_LongAxisBBMax;
	Vect3f m_Sphere_10SegBBMax;
	Vect3f m_ConeBBMax;
	
	Vect3f m_Sphere_10SegBSCenter;
	Vect3f m_ConeBSCenter;

	UAB_BUILD_GET_SET(CRenderableObjectTechnique *, EffectTechnique);
};

#endif

#endif