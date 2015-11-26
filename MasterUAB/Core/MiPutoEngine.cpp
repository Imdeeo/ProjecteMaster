#include "MiPutoEngine.h"


MiPutoEngine::MiPutoEngine(void)
{
}


MiPutoEngine::~MiPutoEngine(void)
{
}

MiPutoEngine* MiPutoEngine::m_Instance = nullptr;

MiPutoEngine* MiPutoEngine::GetInstance()
{
	if(m_Instance == nullptr)
	{
		m_Instance = new  MiPutoEngine();
	}
	return m_Instance;
}

void MiPutoEngine::Init()
{
	m_EffectManager = new CEffectManager();
	m_MaterialManager = new CMaterialManager();
	m_TextureManager = new CTextureManager();
	m_RenderManager = new CRenderManager();

//	m_EffectManager->Load("effects.xml");
//	m_MaterialManager->Load("materials.xml");
}