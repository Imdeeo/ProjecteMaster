#ifndef UABENGINE_H
#define UABENGINE_H

#include "Effects\EffectManager.h"
#include "Materials\MaterialManager.h"
#include "Texture\TextureManager.h"
#include "RenderManager\RenderManager.h"
#include "StaticMesh\StaticMeshManager.h"
#include "Lights\LightManager.h"
#include "RenderableObjects\RenderableObjectManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "ScriptManager\ScriptManager.h"
#include "Camera\CameraControllerManager.h"

class CUABEngine
{

	CStaticMeshManager * m_StaticMeshManager;
	CRenderableObjectsManager * m_RenderableObjectsManager;
	CMaterialManager * m_MaterialManager;
	CEffectManager * m_EffectManager;
	CTextureManager * m_TextureManager;
	CLightManager * m_LightManager;
	CRenderManager * m_RenderManager;
	CAnimatedModelsManager * m_AnimatedModelsManager;
	CScriptManager * m_ScriptManager;
	CCameraControllerManager * m_CameraManager;
	
	UAB_GET_PROPERTY(CStaticMeshManager *, StaticMeshManager)
	UAB_GET_PROPERTY(CRenderableObjectsManager *, RenderableObjectsManager)
	UAB_GET_PROPERTY(CMaterialManager *, MaterialManager)
	UAB_GET_PROPERTY(CEffectManager *, EffectManager)
	UAB_GET_PROPERTY(CTextureManager *, TextureManager)
	UAB_GET_PROPERTY(CLightManager *, LightManager)
	UAB_GET_PROPERTY(CRenderManager *, RenderManager)
	UAB_GET_PROPERTY(CAnimatedModelsManager *, AnimatedModelsManager)
	UAB_GET_PROPERTY(CScriptManager *, ScriptManager)
	UAB_GET_PROPERTY(CCameraControllerManager *, CameraManager)

	static CUABEngine * m_Instance;
	
	CUABEngine(void);
	
public:

	static CUABEngine * GetInstance();
	virtual ~CUABEngine(void);

	void Destroy();
	void Init();
};

#define UABEngine (*(CUABEngine::GetInstance()))

#endif //UABENGINE_H