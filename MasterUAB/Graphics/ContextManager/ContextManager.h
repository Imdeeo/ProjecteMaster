#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include "Effects\Effect.h"
#include "Camera\Camera.h"
#include "Effects\EffectParameters.h"
#include "Effects\EffectManager.h"

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
		BLEND_ALPHA,
		BLEND_DEFERRED,
		
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

	//void SetBaseColor(const CColor& _Color) { CEffectManager::m_LightParameters.m_LightColor = _Color; }
	void SetWorldMatrix(const Mat44f& _Model) { CEffectManager::m_SceneParameters.m_World = _Model; }
	void SetCamera(const Mat44f& _View, const Mat44f& _Projection) { CEffectManager::m_SceneParameters.m_View = _View; CEffectManager::m_SceneParameters.m_Projection = _Projection; }
	void SetCamera(const CCamera& _Camera) { CEffectManager::m_SceneParameters.m_View = _Camera.GetView(); CEffectManager::m_SceneParameters.m_Projection = _Camera.GetProjection(); CEffectManager::m_SceneParameters.m_CameraPosition = _Camera.GetPosition(); }
	//void SetDebugSize(float _Size) { CEffectManager::m_SceneParameters.m_DebugRenderScale = _Size; }

	void Present();
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

	int m_Width, m_Height;
	D3D11_VIEWPORT* m_ViewPort;

	ID3D11RasterizerState*	m_RasterizerSates[RS_COUNT];
	ID3D11DepthStencilState * m_DepthStencilStates[DSS_COUNT];
	ID3D11BlendState* m_BlendStates[BLEND_COUNT];

	int								m_NumViews;
	ID3D11RenderTargetView* const*	m_CurrentRenderTargetViews;
	ID3D11DepthStencilView*			m_CurrentDepthStencilView;


public:
	ID3D11RasterizerState*	GetRasterizerState(ERasterizedState _RasterizedState){return m_RasterizerSates[_RasterizedState];}
	ID3D11DepthStencilState * GetDepthStencilState(EDepthStencilStates _DepthStencilState){return m_DepthStencilStates[_DepthStencilState];}
	ID3D11BlendState* GetBlendState(EBlendStates _BlendState){return m_BlendStates[_BlendState];}

	D3D11_VIEWPORT *getViewPort(){ return m_ViewPort; }
	UAB_GET_PROPERTY(IDXGISwapChain*, SwapChain);
	UAB_GET_PROPERTY(int, Width);
	UAB_GET_PROPERTY(int, Height);
	UAB_GET_PROPERTY(int, NumViews)
	UAB_GET_PROPERTY(ID3D11RenderTargetView* const*, CurrentRenderTargetViews)
	UAB_GET_PROPERTY(ID3D11DepthStencilView*, CurrentDepthStencilView)
	void Clear(bool renderTarget, bool depthStencil, CColor backgroundColor = CColor(.2f, .1f, .4f));
	void Unset();
	void SetRenderTargets(int _NumViews, ID3D11RenderTargetView *const*_RenderTargetViews,ID3D11DepthStencilView *_DepthStencilView);
};

#endif