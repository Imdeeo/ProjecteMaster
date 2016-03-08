#ifndef CAPTURED_FRAME_BUFFER_TEXTURE_H
#define CAPTURED_FRAME_BUFFER_TEXTURE_H
#include "Texture.h"

class ID3D11Texture2D;

class CCapturedFrameBufferTexture :
	public CTexture
{
protected:
	ID3D11Texture2D					*m_DataTexture;

	unsigned int m_Height;
	unsigned int m_Width;

	void Init(const std::string &_Name, unsigned int _Width, unsigned int _Height);
	void Unload();
	bool CreateSamplerState();
public:
	bool Reload();
	CCapturedFrameBufferTexture(const CXMLTreeNode &_TreeNode);
	CCapturedFrameBufferTexture(const std::string &_Name, unsigned int _Width, unsigned int _Height);
	virtual ~CCapturedFrameBufferTexture();

	bool Capture(unsigned int StageId);
};

#endif //CAPTURED_FRAME_BUFFER_TEXTURE_H