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
		RS_SOLID_NO_CULL,
		RS_SOLID_FRONT_CULL,
		RS_SOLID_BACK_CULL,
		// TODO crear un modo que haga culling de la cara frontal y otra de la cara trasera

		RS_COUNT
	};

	enum EDepthStencilStates
	{
		DSS_DEPTH_ON,
		DSS_OFF,
		DSS_TEST_NO_WRITE,
		DSS_TEST_WRITE,
		// TODO: Crear un modo que haga el depth test, pero no escriba su posición

		DSS_COUNT
	};

	enum EBlendStates
	{
		BLEND_SOLID,
		BLEND_CLASSIC,
		BLEND_PREMULT,
		
		// NOTA: Si se añaden más estados aquí, modificar CMaterial::HasBlending

		BLEND_COUNT
	};

public:
	CContextManager();
	~CContextManager();

	void Dispose();

	void Resize(HWND hWnd, unsigned int Width, unsigned int Height);

	HRESULT CreateContext(HWND hWnd, int Width, int Height);
	HRESULT CreateBackBuffer(HWND hWnd, int Width, int Height);
	void InitStates();

	float GetAspectRatio() const { return (float)m_Width / (float)m_Height; }

	void BeginRender(CColor backgroundColor = CColor(.2f, .1f, .4f));
	void EndRender();
	void Draw(const CRenderableVertexs* _VerticesToRender, ERasterizedState _RS = RS_SOLID_FRONT_CULL, EDepthStencilStates _DSS = DSS_DEPTH_ON, EBlendStates _BS = BLEND_SOLID);

	ID3D11Device* GetDevice() const { return m_D3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }

	void SetBaseColor(const CColor& _Color) { m_Parameters.m_BaseColor = _Color; }
	void SetWorldMatrix(const Mat44f& _Model) { m_Parameters.m_World = _Model; }
	void SetCamera(const Mat44f& _View, const Mat44f& _Projection) { m_Parameters.m_View = _View; m_Parameters.m_Projection = _Projection; }
	void SetCamera(const CCamera& _Camera) { m_Parameters.m_View = _Camera.GetView(); m_Parameters.m_Projection = _Camera.GetProjection(); }
	void SetDebugSize(float _Size) { m_Parameters.m_DebugRenderScale = _Size; }

private:

	void InitRasterizerStates();
	void InitDepthStencilStates();
	void InitBlendStates();

	ID3D11Device*			m_D3DDevice;
	ID3D11DeviceContext*	m_DeviceContext;
	ID3D11Debug*			m_D3DDebug;
	IDXGISwapChain*			m_SwapChain;
	ID3D11RenderTargetView*	m_RenderTargetView;
	ID3D11Texture2D*		m_DepthStencil;
	ID3D11DepthStencilView*	m_DepthStencilView;

	CEffectParameters m_Parameters;

	int m_Width, m_Height;

	ID3D11RasterizerState*	m_RasterizerSates[RS_COUNT];
	ID3D11DepthStencilState * m_DepthStencilStates[DSS_COUNT];
	ID3D11BlendState* m_BlendStates[BLEND_COUNT];
};

