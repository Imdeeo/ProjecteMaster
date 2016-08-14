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
#include "VideoManager\VideoManager.h"
#include "GamePlayManager.h"
#include "LevelManager\LevelManager.h"
#include "Manchas\ManchasManager.h"
#include "DebugHelper\DebugHelper.h"
#include "ContextManager\ContextManager.h"
#include "RenderableObjects\RenderableVertexs.h"
#ifdef _DEBUG
#include "DebugRender.h"
#else
#include "RenderHelper\RenderHelper.h"
#endif

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
	m_VideoManager = IVideoManager::InstantiateVideoManager();
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
	//CHECKED_DELETE(m_SoundManager);
	CHECKED_DELETE(m_VideoManager);
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
	m_VideoManager->Update(l_ElapsedTime);
	const CCamera *l_CurrentCamera = m_RenderManager->GetCurrentCamera();
	//GetSoundManager()->Update(l_CurrentCamera);
	m_ScriptManager->RunCode("luaGui()");
}
void CUABEngine::LoadScreen(const std::string _FileName)
{
	std::string l_EffectName;
	CEffectVertexShader* l_EffectVertexShader;
	CEffectPixelShader* l_EffectPixelShader;
	CEffectTechnique* l_EffectTechnique;
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(_FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["load_screen"];
		if (l_Input.Exists())
		{
			
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("vertex_shader"))
				{
					l_EffectVertexShader = new CEffectVertexShader(l_Element);
					l_EffectVertexShader->Load();
				}
				else if (l_Element.GetName() == std::string("pixel_shader"))
				{
					l_EffectPixelShader = new CEffectPixelShader(l_Element);
					l_EffectPixelShader->Load();
				}			
				else if (l_Element.GetName() == std::string("effect_technique"))
				{
					l_EffectName = l_Element.GetPszProperty("name");
					l_EffectTechnique = new CEffectTechnique(l_EffectVertexShader, l_EffectPixelShader, nullptr, l_EffectName);
				}
			}
			CTexture* l_Texture = new CTexture();
			l_Texture->Load("Data\\GUI\\textures\\Carga.png");
			CContextManager* l_ContextManager = m_RenderManager->GetContextManager();
			
			l_ContextManager->BeginRender();			
			m_RenderManager->SetMatrixViewProjection();
			m_RenderManager->Clear(true, true);
			m_RenderManager->GetContextManager()->SetWorldMatrix(m44fIDENTITY);
			CEffectManager::SetSceneConstants(l_EffectTechnique);
			m_RenderManager->DrawScreenQuad(l_EffectTechnique, l_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
			l_ContextManager->EndRender();
		}
	}
}
void CUABEngine::Init()
{	
	m_RenderManager->Init();
	LoadScreen("Data\\effects.xml");
	m_InputManager->Load("Data\\input.xml");
	m_LevelManager->LoadFile("Data\\level.xml");
	m_PhysXManager->LoadPhysx("Data\\physx.xml");
	m_EffectManager->Load("Data\\effects.xml");
	m_RenderableObjectTechniqueManager->Load("Data\\renderable_objects_techniques.xml");
	m_AnimatedModelsManager->Load("Data\\animated_models.xml");
	
	//m_LevelManager->LoadLevel("Biblioteca");
	m_GUIManager->Load("Data\\GUI\\gui_elements.xml");

	
	m_ScriptManager->Initialize();
	m_MaterialManager->Load("Data\\default_effect_materials.xml");
	m_LayerManager->Load("Data\\layers.xml");
	m_SceneRendererCommandManager->Load("Data\\scene_renderer_commands.xml");
	
	/*m_SoundManager->SetPath("Data\\Sounds\\");
	m_SoundManager->Init();
	m_SoundManager->Load("soundbanks.xml", "speakers.xml");*/
#ifdef _DEBUG
	m_RenderManager->GetDebugRender()->SetEffectTechnique(UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_grid"));
#else
	m_RenderManager->GetRenderHelper()->SetEffectTechnique(UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_grid"));
#endif
	m_ScriptManager->RunFile("Data\\Lua\\init.lua");
	UABEngine.GetScriptManager()->RunCode("mainLua()");
	//m_LevelManager->ReloadAllLua();
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

void CUABEngine::ReloadLUA()
{
	LuaIsReloaded();
	m_ScriptManager->Destroy();
	m_GamePlayManager->Clear();
	m_ScriptManager->Initialize();
	m_ScriptManager->RunFile("Data\\Lua\\init.lua");
	UtilsLog("Reloading Lua");
	m_LevelManager->ReloadAllLua();
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
UAB_GET_PROPERTY_CPP(CUABEngine, IVideoManager *, VideoManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CGamePlayManager *, GamePlayManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CLevelManager *, LevelManager)
UAB_GET_PROPERTY_CPP(CUABEngine, CManchasManager *, ManchasManager)
