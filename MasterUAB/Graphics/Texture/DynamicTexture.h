#ifndef DYNAMIC_TEXTURE_H
#define DYNAMIC_TEXTURE_H

#include <D3D11.h>
#include "Utils\Named.h"

class CDynamicTexture :
	public CNamed
{
private:
	ID3D11DepthStencilView* m_DepthStencilView;
public:
	CDynamicTexture();
	virtual ~CDynamicTexture();
	
	ID3D11DepthStencilView* GetDepthStencilView(){ return m_DepthStencilView; }
};

#endif