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
<<<<<<< HEAD
<<<<<<< HEAD
	CCameraControllerManager * m_CameraManager;
=======
	CCameraControllerManager * m_CameraControllerManager;
>>>>>>> develop
	
=======
	CCameraControllerManager * m_CameraManager;
	CCinematic * m_Cinematic;
	CPhysXManager * m_PhysXManager;
	CRenderableObjectTechniqueManager* m_RenderableObjectTechniqueManager;

	std::string m_LevelLoaded;

>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
	UAB_GET_PROPERTY(CStaticMeshManager *, StaticMeshManager)
	UAB_GET_PROPERTY(CLayerManager *, LayerManager)
	UAB_GET_PROPERTY(CMaterialManager *, MaterialManager)
	UAB_GET_PROPERTY(CEffectManager *, EffectManager)
	UAB_GET_PROPERTY(CTextureManager *, TextureManager)
	UAB_GET_PROPERTY(CLightManager *, LightManager)
	UAB_GET_PROPERTY(CRenderManager *, RenderManager)
	UAB_GET_PROPERTY(CAnimatedModelsManager *, AnimatedModelsManager)
	UAB_GET_PROPERTY(CScriptManager *, ScriptManager)
<<<<<<< HEAD
<<<<<<< HEAD
	UAB_GET_PROPERTY(CCameraControllerManager *, CameraManager)
=======
	UAB_GET_PROPERTY(CCameraControllerManager *, CameraControllerManager)
>>>>>>> develop
=======
	UAB_GET_PROPERTY(CCameraControllerManager *, CameraManager)
	UAB_GET_PROPERTY(CCinematic *, Cinematic)
	UAB_GET_PROPERTY(CPhysXManager *, PhysXManager)
	UAB_GET_PROPERTY(CRenderableObjectTechniqueManager *, RenderableObjectTechniqueManager)
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7

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
