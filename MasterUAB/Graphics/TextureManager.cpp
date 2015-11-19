#include "TextureManager.h"


CTextureManager::CTextureManager(void)
{
}


CTextureManager::~CTextureManager(void)
{
}

CTexture * CTextureManager::GetTexture(const std::string &Filename)
{
	CTexture* l_texture = GetResource(Filename);
	if(l_texture==nullptr)
	{
		l_texture = new CTexture();
		l_texture->Load(Filename);
	}
	return l_texture;
}

void CTextureManager::Reload()
{
	for(TMapResource::iterator iterator = m_Resources.begin();iterator != m_Resources.end();iterator++)
	{
		iterator->second->Reload();
	}
}