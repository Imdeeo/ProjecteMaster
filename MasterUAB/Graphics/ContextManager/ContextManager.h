#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include "Math\MathTypes.h"
#include "Math\Color.h"

typedef struct HWND__ *HWND;
typedef _Return_type_success_(return >= 0) long HRESULT;

class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11Debug;
class IDXGISwapChain;
class ID3D11RenderTargetView;
class ID3D11Texture2D;
class ID3D11DepthStencilView;
class ID3D11RenderTargetView;
class ID3D11RasterizerState;
class ID3D11DepthStencilState;
class ID3D11BlendState;
class D3D11_VIEWPORT;
class ID3D11DepthStencilView;

class CRenderableVertexs;
class CCamera;
class CColor;

#define MAX_RENDER_TARGETS 5

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
	void SetWorldMatrix(const Mat44f& _Model);
	void SetCamera(const CCamera& _Camera);
	void CContextManager::SetCamera(const Mat44f& _View, const Mat44f& _Projection);
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

	ID3D11RenderTargetView *m_CurrentRenderTargetViews[MAX_RENDER_TARGETS];

	int m_Width, m_Height;
	D3D11_VIEWPORT* m_ViewPort;

	ID3D11RasterizerState*	m_RasterizerSates[RS_COUNT];
	ID3D11DepthStencilState * m_DepthStencilStates[DSS_COUNT];
	ID3D11BlendState* m_BlendStates[BLEND_COUNT];

	int								m_NumViews;
	ID3D11DepthStencilView*			m_CurrentDepthStencilView;


public:
	ID3D11RasterizerState*	GetRasterizerState(ERasterizedState _RasterizedState){return m_RasterizerSates[_RasterizedState];}
	ID3D11DepthStencilState * GetDepthStencilState(EDepthStencilStates _DepthStencilState){return m_DepthStencilStates[_DepthStencilState];}
	ID3D11BlendState* GetBlendState(EBlendStates _BlendState){return m_BlendStates[_BlendState];}

	D3D11_VIEWPORT * getViewPort(){ return m_ViewPort; }
	void setViewPort(D3D11_VIEWPORT *ViewPort){ m_ViewPort=ViewPort; }
	IDXGISwapChain* GetSwapChain();
	int GetWidth(){ return m_Width; }
	int GetHeight(){ return m_Height; }
	int GetNumViews(){ return m_NumViews; }
	ID3D11RenderTargetView* const* GetCurrentRenderTargetViews();
	ID3D11DepthStencilView* GetCurrentDepthStencilView();
	ID3D11DepthStencilView* GetDepthStencilView();
	void Clear(bool renderTarget, bool depthStencil, CColor backgroundColor = CColor(.0f, .0f, .0f));
	void UnsetRenderTargets();
	void SetRenderTargets(int _NumViews, ID3D11RenderTargetView **_RenderTargetViews, ID3D11DepthStencilView *_DepthStencilView);
};

#endif