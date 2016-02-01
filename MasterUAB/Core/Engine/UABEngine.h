#ifndef UAB_ENGINE_H
#define UAB_ENGINE_H

#define LEVEL "2"

#include "Effects\EffectManager.h"
#include "Materials\MaterialManager.h"
#include "Texture\TextureManager.h"
#include "RenderManager\RenderManager.h"
#include "StaticMesh\StaticMeshManager.h"
#include "Lights\LightManager.h"
#include "Layers\LayerManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "ScriptManager\ScriptManager.h"
#include "Camera\CameraControllerManager.h"
#include "Cinematics\Cinematic.h"
#include "PhysXManager\PhysXManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "SceneRender\SceneRendererCommandManager.h"


#include <string>

class CUABEngine
{

	CStaticMeshManager * m_StaticMeshManager;
	CLayerManager * m_LayerManager;
	CMaterialManager * m_MaterialManager;
	CEffectManager * m_EffectManager;
	CTextureManager * m_TextureManager;
	CLightManager * m_LightManager;
	CRenderManager * m_RenderManager;
	CAnimatedModelsManager * m_AnimatedModelsManager;
	CScriptManager * m_ScriptManager;
	CCameraControllerManager * m_CameraControllerManager;
	CCinematic * m_Cinematic;
	CPhysXManager * m_PhysXManager;
	CRenderableObjectTechniqueManager* m_RenderableObjectTechniqueManager;
	CSceneRendererCommandManager* m_SceneRendererCommandManager;
	

	std::string m_LevelLoaded;
	UAB_GET_PROPERTY(CStaticMeshManager *, StaticMeshManager)
	UAB_GET_PROPERTY(CLayerManager *, LayerManager)
	UAB_GET_PROPERTY(CMaterialManager *, MaterialManager)
	UAB_GET_PROPERTY(CEffectManager *, EffectManager)
	UAB_GET_PROPERTY(CTextureManager *, TextureManager)
	UAB_GET_PROPERTY(CLightManager *, LightManager)
	UAB_GET_PROPERTY(CRenderManager *, RenderManager)
	UAB_GET_PROPERTY(CAnimatedModelsManager *, AnimatedModelsManager)
	UAB_GET_PROPERTY(CScriptManager *, ScriptManager)
	UAB_GET_PROPERTY(CCameraControllerManager *, CameraControllerManager)
	UAB_GET_PROPERTY(CCinematic *, Cinematic)
	UAB_GET_PROPERTY(CPhysXManager *, PhysXManager)
	UAB_GET_PROPERTY(CRenderableObjectTechniqueManager *, RenderableObjectTechniqueManager)
	UAB_GET_PROPERTY(CSceneRendererCommandManager *, SceneRendererCommandManager)

	static CUABEngine * m_Instance;

	std::string GetLevelLoaded()const{return m_LevelLoaded;}
	
	CUABEngine(void);

	void LoadLevelXML(std::string filename);
	
public:

	static CUABEngine * GetInstance();
	virtual ~CUABEngine(void);

	void Destroy();
	void Init();

};

#define UABEngine (*(CUABEngine::GetInstance()))

#endif //UAB_ENGINE_H
