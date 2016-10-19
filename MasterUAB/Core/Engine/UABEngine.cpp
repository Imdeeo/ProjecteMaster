
#include "Engine\UABEngine.h"
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
#include "Camera\FPSCameraController.h"
#include "Cinematics\CinematicManager.h"
#include "PhysXManager\PhysXManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Particles\ParticleManager.h"
#include "GUIManager.h"
#include "GUIPosition.h"
#include "SoundManager\SoundManager.h"
#include "VideoManager\VideoManager.h"
#include "GamePlayManager.h"
#include "LevelManager\LevelManager.h"
#include "Manchas\ManchasManager.h"
#include "DebugHelper\DebugHelper.h"
#include "ContextManager\ContextManager.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "IA\AStarManager.h"
#include "Math\Color.h"
#include "LoadScreen\LoadScreenManager.h"
#ifdef _DEBUG
#include "DebugRender.h"
#else
#include "RenderHelper\RenderHelper.h"
#endif

#include <thread>

CUABEngine::CUABEngine(void) : m_RandomEngine(rnd()), m_UnitDistribution(0.0f, 1.0f)
{
	m_TimeScale = 1;
	m_CurrentCamera_vision = 1;
	m_Pause = false; //Iniciara en false
	m_InputManager = new CInputManager();
	m_EffectManager = new CEffectManager();
	m_MaterialManager = new CMaterialManager();
	m_TextureManager = new CTextureManager();
	m_RenderManager = new CRenderManager();
	m_AnimatedModelsManager = new CAnimatedModelsManager();
	m_ScriptManager = new CScriptManager();
	m_CameraControllerManager = new CCameraControllerManager();
	m_PhysXManager = CPhysXManager::CreatePhysXManager();
	m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
	m_GUIManager = new CGUIManager();
	m_SoundManager = ISoundManager::InstantiateSoundManager();
	m_VideoManager = IVideoManager::InstantiateVideoManager();
	m_FrustumActive = true;
	m_LevelManager = new CLevelManager();
	m_ActiveConsole = false;
}

CUABEngine::~CUABEngine(void)
{
<<<<<<< HEAD

	CHECKED_DELETE(m_LevelManager);
	CHECKED_DELETE(m_SceneRendererCommandManager);
=======
>>>>>>> develop
	CHECKED_DELETE(m_TextureManager);
	CHECKED_DELETE(m_RenderManager);
	CHECKED_DELETE(m_AnimatedModelsManager);
	CHECKED_DELETE(m_CameraControllerManager);
	CHECKED_DELETE(m_RenderManager);
	CHECKED_DELETE(m_MaterialManager);
	CHECKED_DELETE(m_RenderableObjectTechniqueManager);
	CHECKED_DELETE(m_EffectManager);
	CHECKED_DELETE(m_PhysXManager);
	CHECKED_DELETE(m_ScriptManager);
	CHECKED_DELETE(m_GUIManager)
	CHECKED_DELETE(m_SoundManager);
	CHECKED_DELETE(m_VideoManager);
	CHECKED_DELETE(m_InputManager);	
}

CUABEngine* CUABEngine::m_Instance = nullptr;

CUABEngine* CUABEngine::GetInstance()
{
	if(m_Instance == nullptr)
	{
		m_Instance = new  CUABEngine();
	}
	//DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN);
	//DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN);
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
		m_ScriptManager->RunCode("luaUpdate(" + std::to_string(l_ElapsedTime) + ")");
		m_LevelManager->Update(l_ElapsedTime);
	}
	if (m_ActiveConsole)
		Consola(10, 300, 700, 70);
	m_VideoManager->Update(l_ElapsedTime);
	const CCamera *l_CurrentCamera = m_RenderManager->GetCurrentCamera();
	//GetSoundManager()->Update(l_CurrentCamera);
	m_ScriptManager->RunCode("luaGui(" + std::to_string(l_ElapsedTime) + ")");
}

void CUABEngine::Init()
{	
	m_RenderManager->Init();
	m_LoadScreenManager = new CLoadScreenManager("Data\\load_screen.xml");
	//std::thread t(&CLoadScreenManager::Load, m_LoadScreenManager);// , CLoadScreenManager("Data\\load_screen.xml"));

	m_SoundManager->SetPath("Data\\Sounds\\");
	m_SoundManager->Init();
	m_SoundManager->Load("soundbanks.xml", "speakers.xml");
	m_InputManager->Load("Data\\input.xml");
	m_LevelManager->LoadFile("Data\\level.xml");
	m_PhysXManager->LoadPhysx("Data\\physx.xml");
	m_EffectManager->Load("Data\\effects.xml");
	m_RenderableObjectTechniqueManager->Load("Data\\renderable_objects_techniques.xml");
	m_AnimatedModelsManager->Load("Data\\animated_models.xml");
	m_GUIManager->Load("Data\\GUI\\gui_elements.xml");
	m_ScriptManager->Initialize();
	//m_MaterialManager->Load("Data\\default_effect_materials.xml","");
#ifdef _DEBUG
	m_RenderManager->GetDebugRender()->SetEffectTechnique(UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_grid"));
#else
	m_RenderManager->GetRenderHelper()->SetEffectTechnique(UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_grid"));
#endif
	m_ScriptManager->RunFile("Data\\Lua\\init.lua");
	UABEngine.GetScriptManager()->RunCode("mainLua()");

	m_LoadScreenManager->SetLoading(false);
<<<<<<< HEAD
	//t.join();
=======
	t.join();
	CHECKED_DELETE(m_LoadScreenManager);
>>>>>>> develop
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

void CUABEngine::ReloadLUA()
{
	LuaIsReloaded();
	m_ScriptManager->Destroy();
	for (size_t i = 0; i < m_LevelManager->GetResourcesVector().size(); ++i)
	{
		m_LevelManager->GetResourcesVector()[i]->GetGamePlayManager()->Clear();
	}
	m_ScriptManager->Initialize();
	m_ScriptManager->RunFile("Data\\Lua\\init.lua");
	UtilsLog("Reloading Lua");
}

void CUABEngine::Consola(float _x, float _y, float _w, float _h)
{
	static std::string text = "";
	text = m_GUIManager->DoTextBox("console", "fontTest", "> " + text, CGUIPosition(_x, _y, _w, _h));
	text = text.substr(2, text.length() - 2);

	if (text.length() > 0 && text[text.length() - 1] == '\n')
	{
		std::string command = text.substr(0, text.length() - 1);
		m_ScriptManager->RunCode(command);
		m_ActiveConsole = false;
		((CFPSCameraController*)m_CameraControllerManager->GetMainCamera())->Unlock();
		text = "";
	}
}

void CUABEngine::SetActiveConsole(bool _ActiveConsole)
{
	m_ActiveConsole = _ActiveConsole;
	UABEngine.GetInputManager()->GetKeyBoard()->ConsumeLastChar();
}

float CUABEngine::GetRandomValue(float min, float max)
{
	float a = m_UnitDistribution(m_RandomEngine);
	float value = mathUtils::Lerp(min, max, a);
	return value;
}

Vect3f CUABEngine::GetRandomValue(Vect3f min, Vect3f max)
{
	float a1 = m_UnitDistribution(m_RandomEngine);
	float a2 = m_UnitDistribution(m_RandomEngine);
	float a3 = m_UnitDistribution(m_RandomEngine);
	Vect3f value;
	value.x = mathUtils::Lerp(min.x, max.x, a1);
	value.y = mathUtils::Lerp(min.y, max.y, a2);
	value.z = mathUtils::Lerp(min.z, max.z, a3);
	return value;
}

float CUABEngine::GetRandomValue(Vect2f value)
{
	return GetRandomValue(value.x, value.y);
}

CColor CUABEngine::GetRandomValue(CColor min, CColor max)
{
	float a = m_UnitDistribution(m_RandomEngine);
	CColor value = min.Lerp(max, a);
	return value;
}

int CUABEngine::GetTypeParticle(CRenderableObject* _RO)
{
	std::string l_type = _RO->GetTipo();
	if (l_type == "ParticleInstance")
	{
		return 0;
	}
	return 1;
}

UAB_GET_PROPERTY_CPP(CUABEngine, CInputManager *, InputManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CMaterialManager *, MaterialManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CEffectManager *, EffectManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CTextureManager *, TextureManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CRenderManager *, RenderManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CAnimatedModelsManager *, AnimatedModelsManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CScriptManager *, ScriptManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CCameraControllerManager *, CameraControllerManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CPhysXManager *, PhysXManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CRenderableObjectTechniqueManager *, RenderableObjectTechniqueManager)
<<<<<<< HEAD
UAB_GET_PROPERTY_CPP(CUABEngine, CSceneRendererCommandManager *, SceneRendererCommandManager)
=======
>>>>>>> develop
UAB_GET_PROPERTY_CPP(CUABEngine, CGUIManager*, GUIManager)
UAB_GET_PROPERTY_CPP(CUABEngine, ISoundManager *, SoundManager)
UAB_GET_PROPERTY_CPP(CUABEngine, IVideoManager *, VideoManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CLevelManager *, LevelManager)