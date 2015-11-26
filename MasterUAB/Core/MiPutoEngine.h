#ifndef MI_PUTO_ENGINE_H
#define MI_PUTO_ENGINE_H

#include "EffectManager.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "RenderManager.h"

class MiPutoEngine
{
	//UAB_BUILD_GET_SET(CStaticMeshManager *, StaticMeshManager)
	//UAB_BUILD_GET_SET(CRenderableObjectManager *, RenderableObjectManager)
	UAB_BUILD_GET_SET(CMaterialManager *, MaterialManager)
	UAB_BUILD_GET_SET(CEffectManager *, EffectManager)
	UAB_BUILD_GET_SET(CTextureManager *, TextureManager)
	UAB_BUILD_GET_SET(CRenderManager *, RenderManager)

	static MiPutoEngine * m_Instance;
	
	MiPutoEngine(void);
	
public:

	static MiPutoEngine * GetInstance();
	virtual ~MiPutoEngine(void);

	void Init();
};

#endif //MI_PUTO_ENGINE_H