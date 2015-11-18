#include "Named.h"
#include <d3d11.h>

#pragma once
class CTexture : CNamed
{
private:
	ID3D11ShaderResourceView *m_Texture;
	ID3D11SamplerState *m_SamplerState;
	virtual bool LoadFile();
	void Unload();
public:
	CTexture();
	virtual ~CTexture();
	bool Load(const std::string &Filename);
	void Activate(unsigned int StageId);
	bool Reload();
};

