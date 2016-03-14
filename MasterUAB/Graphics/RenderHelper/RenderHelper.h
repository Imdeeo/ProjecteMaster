#ifndef _DEBUG

#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "Math\Vector3.h"

class CEffect;
class ID3D11Device;
class CRenderableObjectTechnique;
class CRenderableVertexs;
class CEffectTechnique;

class CRenderHelper
{
public:
	CRenderHelper(ID3D11Device* _Device);
	~CRenderHelper();

	CRenderableVertexs * GetQuadRV() const { return m_DrawQuadRV; }

	CEffectTechnique *	GetDebugTechnique() const;

private:
	CRenderableObjectTechnique		*m_EffectTechnique;

	CRenderableVertexs				*m_DrawQuadRV;

};

#endif //RENDER_HELPER_H

#endif