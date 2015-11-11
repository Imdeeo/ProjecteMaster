#pragma once

#include <d3d11.h>

#include "Effect.h"
#include "Camera.h"

class CRenderableVertexs;

class CContextManager
{
public:

	enum ERasterizedState
	{
		RS_WIREFRAME,
		RS_SOLID,
		RS_BACKFACES,

		RS_COUNT
	};

public:
	CContextManager();
	~CContextManager();

	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HRESULT CreateContext(HWND hWnd, int Width, int Height);
	HRESULT CreateBackBuffer(HWND hWnd, int Width, int Height);
	void InitStates();


	void BeginRender();
	void EndRender();
	void Draw(CRenderableVertexs* _VerticesToRender, ERasterizedState _RS);

	ID3D11Device* GetDevice() const { return m_D3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }

	void SetBaseColor(const CColor& _Color) { m_Parameters.m_BaseColor = _Color; }
	void SetWorldMatrix(const Mat44f& _Model) { m_Parameters.m_World = _Model; }
	void SetCamera(const Mat44f& _View, const Mat44f& _Projection) { m_Parameters.m_View = _View; m_Parameters.m_Projection = _Projection; }
	void SetCamera(const CCamera& _Camera) { m_Parameters.m_View = _Camera.GetView(); m_Parameters.m_Projection = _Camera.GetProjection(); }
	void SetDebugSize(float _Size) { m_Parameters.m_DebugRenderScale = _Size; }

private:

	void InitRasterizedStates();

	ID3D11Device*			m_D3DDevice;
	ID3D11DeviceContext*	m_DeviceContext;
	IDXGISwapChain*			m_SwapChain;
	ID3D11RenderTargetView*	m_RenderTargetView;
	ID3D11Texture2D*		m_DepthStencil;
	ID3D11DepthStencilView*	m_DepthStencilView;

	CEffectParameters m_Parameters;


	ID3D11RasterizerState*	m_RS[RS_COUNT];
	// TODO ID3D11DepthStencilState
	// TODO ID3D11BlendState
};

