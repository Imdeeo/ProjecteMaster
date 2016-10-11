#ifndef _DEBUG

#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "Math\Vector3.h"
#include "Utils.h"

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
	CRenderableVertexs * GetSPhere10() const { return m_Sphere_10Seg; }
private:
	//CRenderableObjectTechnique		*m_EffectTechnique;

	CRenderableVertexs				*m_DrawQuadRV;

	UAB_BUILD_GET_SET(CRenderableObjectTechnique *, EffectTechnique);
	CRenderableVertexs				*m_Sphere_10Seg;

};

#endif //RENDER_HELPER_H

#endif