#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Texture\Texture.h"
#include "Utils\TemplatedMapManager.h"

#include <map>

class CTextureManager
{
private:
	typedef std::map<std::string, CTemplatedMapManager<CTexture>*> TTextureMap;
	TTextureMap m_TextureMaps;
public:
	CTextureManager(void);
	virtual ~CTextureManager(void);

	CTexture * GetTexture(const std::string &_Filename, const std::string &_LevelId);
	void RemoveTexture(const std::string &_TextureName, const std::string &_LevelId);
	void AddTexture(const std::string &_TextureName, CTexture* _Texture, const std::string &_LevelId);
	void AddUpdateTexture(const std::string &_TextureName, CTexture* _Texture, const std::string &_LevelId);
	void Reload();
};

#endif