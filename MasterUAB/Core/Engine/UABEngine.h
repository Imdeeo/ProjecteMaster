#ifndef MI_PUTO_ENGINE_H
#define MI_PUTO_ENGINE_H

#include "Effects\EffectManager.h"
#include "Materials\MaterialManager.h"
#include "Texture\TextureManager.h"
#include "RenderManager\RenderManager.h"
#include "StaticMesh\StaticMeshManager.h"
#include "Lights\LightManager.h"
#include "RenderableObjects\RenderableObjectManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"

class CUABEngine
{
	UAB_BUILD_GET_SET(CStaticMeshManager *, StaticMeshManager)
	UAB_BUILD_GET_SET(CRenderableObjectsManager *, RenderableObjectsManager)
	UAB_BUILD_GET_SET(CMaterialManager *, MaterialManager)
	UAB_BUILD_GET_SET(CEffectManager *, EffectManager)
	UAB_BUILD_GET_SET(CTextureManager *, TextureManager)
	UAB_BUILD_GET_SET(CLightManager *, LightManager)
	UAB_BUILD_GET_SET(CRenderManager *, RenderManager)
	UAB_BUILD_GET_SET(CAnimatedModelsManager *, AnimatedModelsManager)

	static CUABEngine * m_Instance;
	
	CUABEngine(void);
	
public:

	static CUABEngine * GetInstance();
	virtual ~CUABEngine(void);

	void Init();
};

#define UABEngine (*(CUABEngine::GetInstance()))

#endif //MI_PUTO_ENGINE_H