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
	

	m_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_grid");

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