#ifndef DYNAMIC_TEXTURE_H
#define DYNAMIC_TEXTURE_H

#include "Texture.h"
#include "XML\tinyxml2.h"

#include <string>

class ID3D11RenderTargetView;
class ID3D11DepthStencilView;
class ID3D11Texture2D;	  
class ID3D11RenderTargetView;
class ID3D11DepthStencilView;


class CDynamicTexture : public CTexture
{
protected:
	int m_Width;
	int m_Height;
	int m_Format;

	ID3D11Texture2D *m_RenderTargetTexture;
	ID3D11RenderTargetView *m_RenderTargetView;
	ID3D11Texture2D *m_DepthStencilBuffer;
	ID3D11DepthStencilView *m_DepthStencilView;
	bool m_CreateDepthStencilBuffer;
	void Unload();
	void Init();
	virtual bool CreateSamplerState();	
public:
	CDynamicTexture(const std::string &Name, int Width, int Height, bool CreateDepthStencilBuffer, const std::string &Format);
	CDynamicTexture(tinyxml2::XMLElement* TreeNode);
	~CDynamicTexture();
	bool Reload();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();
	int GetWidth (){return m_Width;}
	int GetHeight(){return m_Height;}
	void SetFormat(const std::string &Format);
	std::string GetFormat();
};

#endif