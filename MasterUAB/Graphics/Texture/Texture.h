#ifndef TEXTURE_H
#define TEXTURE_H

#include "Utils\Named.h"
#include "Utils.h"

class ID3D11ShaderResourceView;
class ID3D11SamplerState;


class CTexture : public CNamed
{
protected:

	
	ID3D11SamplerState *m_SamplerState;
	size_t m_iMaxIndex;
	virtual bool LoadFile();
	
	void Unload();
public:
	CTexture();
	virtual ~CTexture();
	bool Load(const std::string &Filename);
	bool LoadBuffer(unsigned char* _Buffer, size_t _Size);
	void Activate(unsigned int StageId);
	bool Reload();
	ID3D11ShaderResourceView *m_Texture;
	virtual void Save(FILE* _File, size_t NTabs = 0);
	UAB_BUILD_GET_SET(std::string, Type);
};

#endif //TEXTURE_H