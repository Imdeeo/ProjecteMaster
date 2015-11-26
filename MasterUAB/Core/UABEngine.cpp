#include "CUABEngine.h"


CUABEngine::CUABEngine(void)
{
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
	m_EffectManager = new CEffectManager();
	m_MaterialManager = new CMaterialManager();
	m_TextureManager = new CTextureManager();
	m_RenderManager = new CRenderManager();

//	m_EffectManager->Load("effects.xml");
//	m_MaterialManager->Load("materials.xml");
}