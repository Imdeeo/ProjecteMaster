#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Texture\Texture.h"
#include "Utils\TemplatedMapManager.h"

class CTextureManager :	public CTemplatedMapManager<CTexture>
{
public:
	CTextureManager(void);
	virtual ~CTextureManager(void);

	CTexture * GetTexture(const std::string &Filename);
	void Reload();
};

#endif