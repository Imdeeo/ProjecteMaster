#include "Engine\UABEngine.h"
#include <string>



CUABEngine::CUABEngine(void)
{
	m_EffectManager = new CEffectManager();
	m_MaterialManager = new CMaterialManager();
	m_TextureManager = new CTextureManager();
	m_RenderManager = new CRenderManager();
	m_StaticMeshManager = new CStaticMeshManager();
	m_LightManager = new CLightManager();
	m_AnimatedModelsManager = new CAnimatedModelsManager();
	m_RenderableObjectsManager = new CRenderableObjectsManager();
	m_ScriptManager = new CScriptManager();
	m_CameraManager = new CCameraControllerManager();
}


CUABEngine::~CUABEngine(void)
{
	CHECKED_DELETE(m_EffectManager);
	CHECKED_DELETE(m_MaterialManager);
	CHECKED_DELETE(m_TextureManager);
	CHECKED_DELETE(m_RenderManager);
	CHECKED_DELETE(m_StaticMeshManager);
	CHECKED_DELETE(m_LightManager);
	CHECKED_DELETE(m_AnimatedModelsManager);
	CHECKED_DELETE(m_RenderableObjectsManager);
	CHECKED_DELETE(m_ScriptManager);
	CHECKED_DELETE(m_CameraManager);
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
	m_EffectManager->Load("Data\\effects.xml");
	m_MaterialManager->Load("Data\\level_"+std::string(LEVEL)+"\\materials.xml");
	m_StaticMeshManager->Load("Data\\level_"+std::string(LEVEL)+"\\static_meshes.xml");
	m_LightManager->Load("Data\\level_"+std::string(LEVEL)+"\\lights.xml");
	m_AnimatedModelsManager->Load("Data\\animated_models.xml");
	m_RenderableObjectsManager->Load("Data\\level_"+std::string(LEVEL)+"\\renderable_objects.xml");
	m_ScriptManager->Initialize();
	m_CameraManager->Load("Data\\level_"+std::string(LEVEL)+"\\cameras.xml");
}

void CUABEngine::Destroy()
{
	CHECKED_DELETE(m_Instance);
}