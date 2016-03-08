#ifndef DYNAMIC_TEXTURE_H
#define DYNAMIC_TEXTURE_H

#include "Texture.h"

class ID3D11RenderTargetView;
class ID3D11DepthStencilView;
class ID3D11Texture2D;	  
class ID3D11RenderTargetView;
class ID3D11DepthStencilView;

class CXMLTreeNode;

class CDynamicTexture : public CTexture
{
protected:
	int m_Width;
	int m_Height;

	ID3D11Texture2D *m_RenderTargetTexture;
	ID3D11RenderTargetView *m_RenderTargetView;
	ID3D11Texture2D *m_DepthStencilBuffer;
	ID3D11DepthStencilView *m_DepthStencilView;
	bool m_CreateDepthStencilBuffer;
	void Unload();
	void Init();
	virtual bool CreateSamplerState();
public:
	CDynamicTexture(const std::string &Name, int Width, int Height, bool
		CreateDepthStencilBuffer);
	CDynamicTexture(const CXMLTreeNode &TreeNode);
	~CDynamicTexture();
	bool Reload();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();
	int GetWidth (){return m_Width;}
	int GetHeight(){return m_Height;}
};

#endif