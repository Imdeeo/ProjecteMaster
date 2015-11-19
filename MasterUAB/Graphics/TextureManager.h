#include "Texture.h"
#include "TemplatedMapManager.h"

#pragma once
class CTextureManager :	public CTemplatedMapManager<CTexture>
{
public:
	CTextureManager(void);
	virtual ~CTextureManager(void);

	CTexture * GetTexture(const std::string &Filename);
	void Reload();
};

