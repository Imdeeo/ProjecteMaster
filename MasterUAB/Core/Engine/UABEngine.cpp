#include "Engine\UABEngine.h"
#include "XML\XMLTreeNode.h"
#include "RenderableObjects\RenderableObjectsManager.h"

#include "Utils.h"

#include "InputManager\InputManager.h"
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
#include "Cinematics\CinematicManager.h"
#include "PhysXManager\PhysXManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "SceneRender\SceneRendererCommandManager.h"
#include "Particles\ParticleManager.h"
#include "GUIManager.h"
#include "SoundManager\SoundManager.h"
#include "GamePlayManager.h"
#include "LevelManager\LevelManager.h"
#include "Manchas\ManchasManager.h"
#include "DebugHelper\DebugHelper.h"

CUABEngine::CUABEngine(void)
{
	m_TimeScale = 1;
	m_CurrentCamera_vision = 1;
	m_Pause = false; //Iniciara en false
	m_InputManager = new CInputManager();
	m_EffectManager = new CEffectManager();
	m_MaterialManager = new CMaterialManager();
	m_TextureManager = new CTextureManager();
	m_RenderManager = new CRenderManager();
	m_ParticleManager = new CParticleManager();
	m_StaticMeshManager = new CStaticMeshManager();
	m_LightManager = new CLightManager();
	m_AnimatedModelsManager = new CAnimatedModelsManager();
	m_LayerManager = new CLayerManager();
	m_ScriptManager = new CScriptManager();
	m_CameraControllerManager = new CCameraControllerManager();
	m_CinematicManager = new CCinematicManager();
	m_PhysXManager = CPhysXManager::CreatePhysXManager();
	m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
	m_SceneRendererCommandManager = new CSceneRendererCommandManager();
	m_GUIManager = new CGUIManager();
	m_SoundManager = ISoundManager::InstantiateSoundManager();
	m_FrustumActive = true;
	m_GamePlayManager = new CGamePlayManager();
	m_LevelManager = new CLevelManager();
	m_ManchasManager = new CManchasManager();
}

CUABEngine::~CUABEngine(void)
{
	CHECKED_DELETE(m_SceneRendererCommandManager);
	CHECKED_DELETE(m_TextureManager);
	CHECKED_DELETE(m_RenderManager);
	//CHECKED_DELETE(m_Cinematic);
	CHECKED_DELETE(m_LightManager);
	CHECKED_DELETE(m_AnimatedModelsManager);
	CHECKED_DELETE(m_CameraControllerManager);
	CHECKED_DELETE(m_LightManager);
	CHECKED_DELETE(m_StaticMeshManager);
	CHECKED_DELETE(m_LayerManager);
	CHECKED_DELETE(m_RenderManager);
	CHECKED_DELETE(m_ParticleManager);
	CHECKED_DELETE(m_MaterialManager);
	CHECKED_DELETE(m_RenderableObjectTechniqueManager);
	CHECKED_DELETE(m_EffectManager);
	CHECKED_DELETE(m_PhysXManager);
	CHECKED_DELETE(m_ScriptManager);
	CHECKED_DELETE(m_GUIManager)
	CHECKED_DELETE(m_SoundManager);
	CHECKED_DELETE(m_LevelManager);
	CHECKED_DELETE(m_ManchasManager);
	//CHECKED_DELETE(m_CinematicManager);
}

CUABEngine* CUABEngine::m_Instance = nullptr;

CUABEngine* CUABEngine::GetInstance()
{
	if(m_Instance == nullptr)
	{
		m_Instance = new  CUABEngine();
	}
	return m_Instance;
}
void CUABEngine::Update(float _ElapsedTime)
{
	float l_ElapsedTime = _ElapsedTime * m_TimeScale;
	if (l_ElapsedTime > 0.0f && !m_Pause)
	{
		m_RenderManager->SetUseDebugCamera(m_CurrentCamera_vision == 0);
		m_PhysXManager->Update(l_ElapsedTime);
		m_CameraControllerManager->Update(l_ElapsedTime);
		m_RenderManager->SetUseDebugCamera(m_CurrentCamera_vision == 0);
		m_LayerManager->Update(l_ElapsedTime);
		m_ScriptManager->RunCode("luaUpdate(" + std::to_string(l_ElapsedTime) + ")");
		m_GamePlayManager->Update(l_ElapsedTime);
	}
	const CCamera *l_CurrentCamera = m_RenderManager->GetCurrentCamera();
	GetSoundManager()->Update(l_CurrentCamera);
	m_ScriptManager->RunCode("luaGui()");
}
void CUABEngine::Init()
{
	m_InputManager->Load("Data\\input.xml");
	m_LevelManager->LoadFile("Data\\level.xml");
	m_PhysXManager->LoadPhysx("Data\\physx.xml");
	m_EffectManager->Load("Data\\effects.xml");
	m_RenderableObjectTechniqueManager->Load("Data\\renderable_objects_techniques.xml");
	m_AnimatedModelsManager->Load("Data\\animated_models.xml");
	m_LevelManager->LoadLevel("Recibidor");
	m_LevelManager->LoadLevel("Biblioteca");
	m_GUIManager->Load("Data\\GUI\\gui_elements.xml");
	m_ScriptManager->Initialize();
	m_MaterialManager->Load("Data\\default_effect_materials.xml");
	m_SceneRendererCommandManager->Load("Data\\scene_renderer_commands.xml");
	m_RenderManager->Init();
	m_SoundManager->SetPath("Data\\Sounds\\");
	m_SoundManager->Init();
	m_SoundManager->Load("soundbanks.xml", "speakers.xml");

	m_ScriptManager->RunFile("Data\\Lua\\init.lua");
	m_LevelManager->ReloadAllLua();

	// INICIO TIEMPO TEST LECTURA XML
	//float l_StartTime = (float)timeGetTime();
	/*LoadLevelXML("Data\\level.xml");
	m_InputManager->Load("Data\\input.xml");
	m_PhysXManager->LoadPhysx("Data\\physx.xml");
	m_EffectManager->Load("Data\\effects.xml");
	m_RenderableObjectTechniqueManager->Load("Data\\renderable_objects_techniques.xml");
	m_MaterialManager->Load("Data\\level_" + m_LevelLoaded + "\\materials.xml", "Data\\default_effect_materials.xml");
	m_ParticleManager->Load("Data\\level_" + m_LevelLoaded + "\\particles.xml");
	m_ManchasManager->Load("Data\\level_" + m_LevelLoaded + "\\cordura.xml");
	m_StaticMeshManager->Load("Data\\level_" + m_LevelLoaded + "\\static_meshes.xml");
	m_AnimatedModelsManager->Load("Data\\animated_models.xml");	
	m_LayerManager->Load("Data\\level_" + m_LevelLoaded + "\\renderable_objects.xml");	
	m_LightManager->Load("Data\\level_"+m_LevelLoaded+"\\lights.xml");	
	m_CinematicManager->LoadXML("Data\\level_" + m_LevelLoaded + "\\cinematic.xml");
	m_LayerManager->GetLayer()->AddResource("Cinematic", m_CinematicManager);
	m_GUIManager->Load("Data\\GUI\\gui_elements.xml");
	m_ScriptManager->Initialize();
	m_CameraControllerManager->Load("Data\\level_"+m_LevelLoaded+"\\cameras.xml");
	m_SceneRendererCommandManager->Load("Data\\scene_renderer_commands.xml");
	m_RenderManager->Init();
	m_SoundManager->SetPath("Data\\Sounds\\");
	m_SoundManager->Init();
	m_SoundManager->Load("soundbanks.xml", "speakers.xml");

	m_ScriptManager->RunFile("Data\\Lua\\init.lua");
	m_ScriptManager->RunCode("mainLua(\""+m_LevelLoaded+"\")");*/
	// TEST LECTURA XML
	/*float l_EndTime = (float)timeGetTime();
	float l_LoadTimer = l_EndTime - l_StartTime;
	std::ostringstream ss;
	ss << l_LoadTimer;
	std::string s(ss.str());
	CDebugHelper::GetDebugHelper()->Log(s);*/
}
void CUABEngine::Destroy()
{
	CHECKED_DELETE(m_Instance);
}
void CUABEngine::SwitchCamera()
{
	m_CurrentCamera_vision++;
	m_CurrentCamera_vision = m_CurrentCamera_vision % 2;
	UABEngine.GetCameraControllerManager()->SetCurrentCameraControl(m_CurrentCamera_vision);
}
void CUABEngine::ChangeCameraVision()
{
	m_CurrentCamera_vision++;
	m_CurrentCamera_vision = m_CurrentCamera_vision % 2;
}
void CUABEngine::Quit()
{
	PostQuitMessage(0);
}
UAB_GET_PROPERTY_CPP(CUABEngine, CInputManager *, InputManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CStaticMeshManager *, StaticMeshManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CLayerManager *, LayerManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CMaterialManager *, MaterialManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CEffectManager *, EffectManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CTextureManager *, TextureManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CLightManager *, LightManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CRenderManager *, RenderManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CAnimatedModelsManager *, AnimatedModelsManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CScriptManager *, ScriptManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CCameraControllerManager *, CameraControllerManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CCinematicManager *, CinematicManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CPhysXManager *, PhysXManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CRenderableObjectTechniqueManager *, RenderableObjectTechniqueManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CSceneRendererCommandManager *, SceneRendererCommandManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CParticleManager*, ParticleManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CGUIManager*, GUIManager)
UAB_GET_PROPERTY_CPP(CUABEngine, ISoundManager *, SoundManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CGamePlayManager *, GamePlayManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CLevelManager *, LevelManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CManchasManager *, ManchasManager)
