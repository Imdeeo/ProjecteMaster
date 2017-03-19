#include "DynamicTextureManager.h"
#include "DynamicTexture.h"
#include "Materials\Material.h"

CDynamicTextureManager::CDynamicTextureManager(CDynamicTexture *_m_Texture, CMaterial *_m_Material) : m_Texture(_m_Texture), m_Material(_m_Material)
{
}

CDynamicTextureManager::~CDynamicTextureManager()
{
	Destroy();
}

void CDynamicTextureManager::Destroy()
{
	/*CHECKED_RELEASE(m_Texture);
	CHECKED_RELEASE(m_Material);*/
}

CDynamicTexture* CDynamicTextureManager::GetTexture()
{
	return m_Texture;
}
CMaterial* CDynamicTextureManager::GetMaterial()
{
	return m_Material;
}