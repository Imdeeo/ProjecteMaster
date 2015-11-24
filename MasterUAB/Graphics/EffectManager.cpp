#include "EffectManager.h"


CEffectManager::CEffectManager(void)
{
}


CEffectManager::~CEffectManager(void)
{
}

void CEffectManager::Reload()
{
	Load(m_Filename);
}
void CEffectManager::Load(const std::string &Filename)
{
	m_Filename = Filename;
}
CEffectVertexShader * CEffectManager::GetVertexShader(const std::string &VertexShader)
{
	return m_VertexShaders[VertexShader];
}
CEffectPixelShader * CEffectManager::GetPixelShader(const std::string &PixelShader)
{
	return m_PixelShaders[PixelShader];
}
