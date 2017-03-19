#ifndef _DEBUG

#include "RenderHelper.h"

#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"

#include "RenderableObjects\VertexTypes.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"
#include "RenderableObjects\TemplatedRenderableIndexedVertexs.h"
#include "Effects\Effect.h"

#include <d3d11.h>


CRenderHelper::CRenderHelper(ID3D11Device* _Device)
{
	{
		// Quad 2D

		MV_POSITION4_COLOR_TEXTURE_VERTEX l_ScreenVertexsQuad[4] =
		{
			{ Vect4f(-1.0f, 1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f),
			Vect2f(0.0f, 0.0f) },
			{ Vect4f(-1.0f, -1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f),
			Vect2f(0.0f, 1.0f) },
			{ Vect4f(1.0f, 1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f),
			Vect2f(1.0f, 0.0f) },
			{ Vect4f(1.0f, -1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f),
			Vect2f(1.0f, 1.0f) }
		};
		m_DrawQuadRV = new CUABTrianglesStripRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(&l_ScreenVertexsQuad[0].Position.x, 4, 2);
	}

	MV_POSITION_COLOR_VERTEX l_Sphere_10SegVtxs[42] =
	{
		{ Vect3f(0.0f, 0.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, 0.587785f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.345491f, 0.475528f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.559017f, 0.181636f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.559017f, -0.181636f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.345492f, -0.475528f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, -0.587785f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.345492f, -0.475528f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.559017f, -0.181635f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.559017f, 0.181636f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.345491f, 0.475529f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, 0.951057f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.559017f, 0.769421f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.904508f, 0.293893f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.904509f, -0.293893f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.559017f, -0.769421f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, -0.951057f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.559017f, -0.769421f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.904509f, -0.293892f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.904508f, 0.293893f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.559016f, 0.769421f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, 0.951056f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.559017f, 0.769421f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.904508f, 0.293893f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.904508f, -0.293893f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.559017f, -0.769421f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, -0.951056f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.559017f, -0.769421f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.904509f, -0.293892f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.904508f, 0.293893f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.559016f, 0.769421f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, 0.587785f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.345491f, 0.475528f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.559017f, 0.181636f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.559017f, -0.181636f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(-0.345492f, -0.475528f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, -0.587785f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.345492f, -0.475528f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.559017f, -0.181635f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.559017f, 0.181636f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.345491f, 0.475529f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vect3f(0.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	uint16_t l_Sphere_10SegIdxs[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 1, 1, 11, 12, 1, 12, 2, 2, 12, 13, 2, 13, 3, 3, 13, 14, 3, 14, 4, 4, 14, 15, 4, 15, 5, 5, 15, 16, 5, 16, 6, 6, 16, 17, 6, 17, 7, 7, 17, 18, 7, 18, 8, 8, 18, 19, 8, 19, 9, 9, 19, 20, 9, 20, 10, 10, 20, 11, 10, 11, 1, 11, 21, 22, 11, 22, 12, 12, 22, 23, 12, 23, 13, 13, 23, 24, 13, 24, 14, 14, 24, 25, 14, 25, 15, 15, 25, 26, 15, 26, 16, 16, 26, 27, 16, 27, 17, 17, 27, 28, 17, 28, 18, 18, 28, 29, 18, 29, 19, 19, 29, 30, 19, 30, 20, 20, 30, 21, 20, 21, 11, 21, 31, 32, 21, 32, 22, 22, 32, 33, 22, 33, 23, 23, 33, 34, 23, 34, 24, 24, 34, 35, 24, 35, 25, 25, 35, 36, 25, 36, 26, 26, 36, 37, 26, 37, 27, 27, 37, 38, 27, 38, 28, 28, 38, 39, 28, 39, 29, 29, 39, 40, 29, 40, 30, 30, 40, 31, 30, 31, 21, 41, 32, 31, 41, 33, 32, 41, 34, 33, 41, 35, 34, 41, 36, 35, 41, 37, 36, 41, 38, 37, 41, 39, 38, 41, 40, 39, 41, 31, 40 };

	m_Sphere_10Seg = new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_Sphere_10SegVtxs, 42, l_Sphere_10SegIdxs, 240);

	

	//m_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_grid");

}
CRenderHelper::~CRenderHelper()
{

	delete m_DrawQuadRV;

}

CEffectTechnique *	CRenderHelper::GetDebugTechnique() const
{
	return m_EffectTechnique->GetEffectTechnique();
}

#endif