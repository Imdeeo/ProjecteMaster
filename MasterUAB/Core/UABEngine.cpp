#include "UABEngine.h"


CUABEngine::CUABEngine(void)
{
	m_EffectManager = new CEffectManager();
	m_MaterialManager = new CMaterialManager();
	m_TextureManager = new CTextureManager();
	m_RenderManager = new CRenderManager();
	m_StaticMeshManager = new CStaticMeshManager();
	m_RenderableObjectsManager = new CRenderableObjectsManager();
}


CUABEngine::~CUABEngine(void)
{
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
	m_EffectManager->Load("Data\\Level\\effects.xml");
	m_MaterialManager->Load("Data\\Level\\materials.xml");
	m_StaticMeshManager->Load("Data\\Level\\static_meshes.xml");
	m_RenderableObjectsManager->Load("Data\\Level\\renderable_objects.xml");
}