#ifndef _CONTEXT_MANAGER_D3D_H
#define _CONTEXT_MANAGER_D3D_H

#include <Windows.h>

class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;

class CContextManagerD3D
{

private:
	static CContextManagerD3D *m_Instance;

	ID3D11Device *m_D3DDevice;
	ID3D11DeviceContext *m_DeviceContext;
	IDXGISwapChain *m_SwapChain;
	ID3D11RenderTargetView *m_RenderTargetView;
	
	//ID3D11Texture2D* m_DepthStencil;
	//ID3D11DepthStencilView*	m_DepthStencilView;
	
	CContextManagerD3D(void);

public:

	virtual ~CContextManagerD3D(void);

	static CContextManagerD3D *GetInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new CContextManagerD3D();
		}
		return m_Instance;
	}

	ID3D11Device *GetDevice() const {return m_D3DDevice;}

	void Render();

	bool InitDevice(HWND OutputWindowInstance, UINT Width, UINT Height);

	void Draw();

	HRESULT CreateRenderTargetView();
	//HRESULT CreateDepthStencil();
	//HRESULT CreateDepthStencilView();
	
};

#endif //_CONTEXT_MANAGER_D3D_H

