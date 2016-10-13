#include "TextureManager.h"




CTextureManager::CTextureManager(void)
{
}


CTextureManager::~CTextureManager(void)
{
	TTextureMap::iterator it = m_TextureMaps.begin();
	for (; it != m_TextureMaps.end(); it++)
	{
		it->second->Destroy();
	}
	m_TextureMaps.clear();
}

CTexture * CTextureManager::GetTexture(const std::string &_Filename, const std::string &_LevelId)
{
	if (m_TextureMaps.find(_LevelId)==m_TextureMaps.end())
	{
		m_TextureMaps[_LevelId] = new CTemplatedMapManager<CTexture>();
	}
	CTexture* l_texture = m_TextureMaps[_LevelId]->GetResource(_Filename);
	if(l_texture==nullptr)
	{
		l_texture = new CTexture();
		l_texture->Load(_Filename);
		m_TextureMaps[_LevelId]->AddResource(_Filename, l_texture);
	}
	return l_texture;
}

void CTextureManager::Reload()
{
	TTextureMap::iterator it = m_TextureMaps.begin();
	for (; it != m_TextureMaps.end(); it++)
	{
		for (CTemplatedMapManager<CTexture>::TMapResource::iterator iterator = it->second->GetResourcesMap().begin(); iterator != it->second->GetResourcesMap().end(); iterator++)
		{
			iterator->second->Reload();
		}
	}
}

void CTextureManager::RemoveTexture(const std::string &_TextureName, const std::string &_LevelId)
{
	m_TextureMaps[_LevelId]->RemoveResource(_TextureName);
}

void CTextureManager::AddTexture(const std::string &_TextureName, CTexture* _Texture, const std::string &_LevelId)
{
	if (m_TextureMaps.find(_LevelId) == m_TextureMaps.end())
	{
		m_TextureMaps[_LevelId] = new CTemplatedMapManager<CTexture>();
	}
	m_TextureMaps[_LevelId]->AddResource(_TextureName,_Texture);
}

void CTextureManager::AddUpdateTexture(const std::string &_TextureName, CTexture* _Texture, const std::string &_LevelId)
{
	if (m_TextureMaps.find(_LevelId) == m_TextureMaps.end())
	{
		m_TextureMaps[_LevelId] = new CTemplatedMapManager<CTexture>();
	}
	m_TextureMaps[_LevelId]->AddUpdateResource(_TextureName,_Texture);
}