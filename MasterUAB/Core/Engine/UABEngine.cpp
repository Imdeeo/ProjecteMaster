#include "Engine\UABEngine.h"
#include "XML\XMLTreeNode.h"
#include "RenderableObjects\RenderableObjectsManager.h"
CUABEngine::CUABEngine(void)
{
	m_EffectManager = new CEffectManager();
	m_MaterialManager = new CMaterialManager();
	m_TextureManager = new CTextureManager();
	m_RenderManager = new CRenderManager();
	m_StaticMeshManager = new CStaticMeshManager();
	m_LightManager = new CLightManager();
	m_AnimatedModelsManager = new CAnimatedModelsManager();
	m_LayerManager = new CLayerManager();
	m_ScriptManager = new CScriptManager();
	m_CameraControllerManager = new CCameraControllerManager();
	m_Cinematic= new CCinematic();
	m_PhysXManager = CPhysXManager::CreatePhysXManager();
	m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
	m_SceneRendererCommandManager = new CSceneRendererCommandManager();
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
	CHECKED_DELETE(m_MaterialManager);
	CHECKED_DELETE(m_RenderableObjectTechniqueManager);
	CHECKED_DELETE(m_EffectManager);	
	CHECKED_DELETE(m_PhysXManager);
	CHECKED_DELETE(m_ScriptManager);
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

void CUABEngine::Init()
{
	LoadLevelXML("Data\\level.xml");	
	m_EffectManager->Load("Data\\effects.xml");
	m_RenderableObjectTechniqueManager->Load("Data\\renderable_objects_techniques.xml");
	m_MaterialManager->Load("Data\\level_" + m_LevelLoaded + "\\materials.xml");
	m_StaticMeshManager->Load("Data\\level_" + m_LevelLoaded + "\\static_meshes.xml");
	m_AnimatedModelsManager->Load("Data\\animated_models.xml");
	m_LayerManager->Load("Data\\level_" + m_LevelLoaded + "\\renderable_objects.xml");	
	m_LightManager->Load("Data\\level_"+m_LevelLoaded+"\\lights.xml");	
	m_Cinematic->LoadXML("Data\\level_"+m_LevelLoaded+"\\cinematic.xml");
	m_LayerManager->GetLayer()->AddResource("Cinematic",m_Cinematic);
	m_ScriptManager->Initialize();
	m_CameraControllerManager->Load("Data\\level_"+m_LevelLoaded+"\\cameras.xml");
	//m_Cinematic->Play("true");
	m_SceneRendererCommandManager->Load("Data\\scene_renderer_commands.xml");
	m_RenderManager->Init();

	m_ScriptManager->RunFile("Data\\Lua\\init.lua");
	m_ScriptManager->RunCode("mainLua()");
}

void CUABEngine::Destroy()
{
	CHECKED_DELETE(m_Instance);
}

void CUABEngine::LoadLevelXML(std::string filename)
{

	CXMLTreeNode l_XML;
	bool isLoading = l_XML.LoadFile(filename.c_str());

	if (isLoading)
	{
		CXMLTreeNode l_Input = l_XML["level"];
		if (l_Input.Exists())
		{
			m_LevelLoaded = l_Input.GetPszProperty("level_to_load");
		}
	}

}