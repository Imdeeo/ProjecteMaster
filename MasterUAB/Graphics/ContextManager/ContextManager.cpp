#include "ContextManager.h"
#include "RenderableObjects\VertexTypes.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "DebugRender.h"
#include "AntTweakBar.h"

#include "Effects\EffectManager.h"

#include "Math\Matrix44.h"
#include "Math\Vector4.h"
#include "Math\Color.h"

#include "Camera\Camera.h"

#include "Engine\UABEngine.h"
#include "MutexManager\MutexManager.h"

#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")

CContextManager::CContextManager()
	: m_D3DDevice(nullptr)
	, m_DeviceContext(nullptr)
	, m_SwapChain(nullptr)
	, m_RenderTargetView(nullptr)
	, m_DepthStencil(nullptr)
	, m_DepthStencilView(nullptr)
	, m_D3DDebug(nullptr)
	,m_CurrentDepthStencilView(nullptr)
{
	for (int i = 0; i < RS_COUNT; ++i)
	{
		m_RasterizerSates[i] = nullptr;
	}
	for (int i = 0; i < DSS_COUNT; ++i)
	{
		m_DepthStencilStates[i] = nullptr;
	}
	for (int i = 0; i < BLEND_COUNT; ++i)
	{
		m_BlendStates[i] = nullptr;
	}
}

CContextManager::~CContextManager()
{
	Dispose();
}

void CContextManager::Dispose()
{
	for (int i = 0; i < RS_COUNT; ++i)
	{
		CHECKED_RELEASE(m_RasterizerSates[i]);
	}
	for (int i = 0; i < DSS_COUNT; ++i)
	{
		CHECKED_RELEASE(m_DepthStencilStates[i]);
	}
	for (int i = 0; i < BLEND_COUNT; ++i)
	{
		CHECKED_RELEASE(m_BlendStates[i]);
	}

	CHECKED_RELEASE(m_RenderTargetView);
	CHECKED_RELEASE(m_DepthStencil);
	CHECKED_RELEASE(m_DepthStencilView);

	if (m_SwapChain)
	{
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_SwapChain->SetFullscreenState(FALSE, nullptr);
	}

	if (m_D3DDebug)
		m_D3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	CHECKED_RELEASE(m_D3DDebug);

	CHECKED_RELEASE(m_D3DDevice);
	CHECKED_RELEASE(m_DeviceContext);
	CHECKED_RELEASE(m_SwapChain);
	CHECKED_DELETE(m_ViewPort);
}

HRESULT CContextManager::CreateContext(HWND hWnd, int Width, int Height)
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Width;
	sd.BufferDesc.Height = Height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

#if _DEBUG
	//int flags = D3D10_CREATE_DEVICE_DEBUG;
	int flags = 0;
#else
	int flags = 0;
#endif
	
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_D3DDevice, NULL, &m_DeviceContext)))
	{
		return S_FALSE;
	}
	HRESULT hr;
#if _DEBUG
	hr = m_D3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_D3DDebug));
	if (FAILED(hr))
		return hr;
#endif
	
	// treure el ALT+INTRO automàtic
	IDXGIFactory* dxgiFactory;
	hr = m_SwapChain->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
	assert(hr == S_OK);

	hr = dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
	assert(hr == S_OK);

	dxgiFactory->Release();

	return S_OK;
}

HRESULT CContextManager::CreateBackBuffer(HWND hWnd, int Width, int Height)
{
	CHECKED_RELEASE(m_RenderTargetView);
	CHECKED_RELEASE(m_DepthStencil);
	CHECKED_RELEASE(m_DepthStencilView);

	m_Width = Width;
	m_Height = Height;

	ID3D11Texture2D *pBackBuffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return FALSE;

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_D3DDevice->CreateTexture2D(&descDepth, NULL, &m_DepthStencil);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_D3DDevice->CreateDepthStencilView(m_DepthStencil, &descDSV, &m_DepthStencilView);
	if (FAILED(hr))
		return hr;

	SetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	CHECKED_DELETE(m_ViewPort);

	m_ViewPort = new D3D11_VIEWPORT();
	m_ViewPort->Width = (FLOAT)m_Width;
	m_ViewPort->Height = (FLOAT)m_Height;
	m_ViewPort->MinDepth = 0.0f;
	m_ViewPort->MaxDepth = 1.0f;
	m_ViewPort->TopLeftX = 0;
	m_ViewPort->TopLeftY = 0;

	return S_OK;
}


void CContextManager::InitStates()
{
	InitRasterizerStates();
	InitDepthStencilStates();
	InitBlendStates();
}

void CContextManager::InitRasterizerStates()
{
	{
		D3D11_RASTERIZER_DESC l_WireframeDesc;
		ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		l_WireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		l_WireframeDesc.CullMode = D3D11_CULL_NONE;
		l_WireframeDesc.FrontCounterClockwise = true;

		HRESULT l_HR = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_WIREFRAME]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_RASTERIZER_DESC l_WireframeDesc;
		ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		l_WireframeDesc.FillMode = D3D11_FILL_SOLID;
		l_WireframeDesc.CullMode = D3D11_CULL_NONE;
		l_WireframeDesc.FrontCounterClockwise = true;

		HRESULT l_HR = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_SOLID_NO_CULL]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_RASTERIZER_DESC l_WireframeDesc;
		ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		l_WireframeDesc.FillMode = D3D11_FILL_SOLID;
		l_WireframeDesc.CullMode = D3D11_CULL_FRONT;
		l_WireframeDesc.FrontCounterClockwise = true;

		HRESULT l_HR = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_SOLID_FRONT_CULL]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_RASTERIZER_DESC l_WireframeDesc;
		ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		l_WireframeDesc.FillMode = D3D11_FILL_SOLID;
		l_WireframeDesc.CullMode = D3D11_CULL_BACK;
		l_WireframeDesc.FrontCounterClockwise = true;

		HRESULT l_HR = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_SOLID_BACK_CULL]);
		assert(l_HR == S_OK);
	}
}

void CContextManager::InitDepthStencilStates()
{
	{
		D3D11_DEPTH_STENCIL_DESC l_desc = {};
		l_desc.DepthEnable = true;
		l_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		l_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT l_HR = m_D3DDevice->CreateDepthStencilState(&l_desc, &m_DepthStencilStates[DSS_DEPTH_ON]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_DEPTH_STENCIL_DESC l_desc = {};
		l_desc.DepthEnable = false;
		l_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		l_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT l_HR = m_D3DDevice->CreateDepthStencilState(&l_desc, &m_DepthStencilStates[DSS_OFF]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_DEPTH_STENCIL_DESC l_desc = {};
		l_desc.DepthEnable = true;
		l_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		l_desc.DepthFunc = D3D11_COMPARISON_LESS;


		HRESULT l_HR = m_D3DDevice->CreateDepthStencilState(&l_desc, &m_DepthStencilStates[DSS_TEST_NO_WRITE]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_DEPTH_STENCIL_DESC l_desc = {};
		l_desc.DepthEnable = true;
		l_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		l_desc.DepthFunc = D3D11_COMPARISON_LESS;


		HRESULT l_HR = m_D3DDevice->CreateDepthStencilState(&l_desc, &m_DepthStencilStates[DSS_TEST_WRITE]);
		assert(l_HR == S_OK);
	}
}

void CContextManager::InitBlendStates()
{
	{
		D3D11_BLEND_DESC l_desc = {};
		l_desc.RenderTarget[0].BlendEnable = false;
		l_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		l_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		l_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		l_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		l_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT l_HR = m_D3DDevice->CreateBlendState(&l_desc, &m_BlendStates[BLEND_SOLID]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_BLEND_DESC l_desc = {};
		l_desc.RenderTarget[0].BlendEnable = true;
		l_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		l_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		l_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		l_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		l_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT l_HR = m_D3DDevice->CreateBlendState(&l_desc, &m_BlendStates[BLEND_CLASSIC]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_BLEND_DESC l_desc = {};
		l_desc.RenderTarget[0].BlendEnable = true;
		l_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		l_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		l_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		l_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		l_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT l_HR = m_D3DDevice->CreateBlendState(&l_desc, &m_BlendStates[BLEND_PREMULT]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_BLEND_DESC l_AlphablendDesc;
		ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
		l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
		l_AlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		l_AlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		l_AlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT l_HR = m_D3DDevice->CreateBlendState(&l_AlphablendDesc, &m_BlendStates[BLEND_ALPHA]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_BLEND_DESC l_AlphablendDesc;
		ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
		l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
		l_AlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		l_AlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		l_AlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;;

		HRESULT l_HR = m_D3DDevice->CreateBlendState(&l_AlphablendDesc, &m_BlendStates[BLEND_DEFERRED]);
	}
	
return;
}

void CContextManager::Resize(HWND hWnd, unsigned int Width, unsigned int Height)
{
	if (m_D3DDevice != nullptr)
	{
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		CHECKED_RELEASE(m_RenderTargetView);
		CHECKED_RELEASE(m_DepthStencil);
		CHECKED_RELEASE(m_DepthStencilView);

		m_SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
		HRESULT hr = CreateBackBuffer(hWnd, Width, Height);
		assert(hr == S_OK);
	}
}

void CContextManager::Draw(const CRenderableVertexs* _VerticesToRender, ERasterizedState _RS, EDepthStencilStates _DSS, EBlendStates _BS)
{
	m_DeviceContext->RSSetState(m_RasterizerSates[_RS]);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilStates[_DSS], 0);
	Vect4f v(1, 1, 1, 1);
	m_DeviceContext->OMSetBlendState(m_BlendStates[_BS], &v.x, 0xffffffff);

	/*s_DebugEffect->UpdateParameters(m_DeviceContext, &m_Parameters);
	s_DebugEffect->SetActive(m_DeviceContext);*/

/*
	_VerticesToRender->SetBuffers(m_DeviceContext);

	_VerticesToRender->Draw(m_DeviceContext);*/
}

void CContextManager::BeginRender(CColor backgroundColor)
{
	/*D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_Width;
	vp.Height = (FLOAT)m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;*/
	m_DeviceContext->RSSetViewports(1, m_ViewPort);

	/*m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, &backgroundColor.x);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);*/
	//m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	//m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

void CContextManager::EndRender()
{
	Present();
	//m_SwapChain->Present(0, 0);
}


void CContextManager::Present()
{
	m_SwapChain->Present(0, 0);
}



void CContextManager::UnsetRenderTargets()
{	
	SetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	UABEngine.GetMutexManager()->g_DeviceContextMutex.lock();
	GetDeviceContext()->RSSetViewports(1, getViewPort());
	UABEngine.GetMutexManager()->g_DeviceContextMutex.unlock();
}

void CContextManager::SetRenderTargets(int _NumViews, ID3D11RenderTargetView **_RenderTargetViews, ID3D11DepthStencilView *_DepthStencilView)
{
	m_NumViews = _NumViews;
	m_CurrentDepthStencilView = _DepthStencilView;
	
	assert(m_NumViews<=MAX_RENDER_TARGETS);
	
	for(int i=0;i<_NumViews;++i)
		m_CurrentRenderTargetViews[i]=_RenderTargetViews[i];

	//m_CurrentRenderTargetViews = _RenderTargetViews;
	UABEngine.GetMutexManager()->g_DeviceContextMutex.lock();
	GetDeviceContext()->OMSetRenderTargets(_NumViews, m_CurrentRenderTargetViews, _DepthStencilView);
	UABEngine.GetMutexManager()->g_DeviceContextMutex.unlock();
}

void CContextManager::Clear(bool renderTarget, bool depthStencil, CColor backgroundColor)
{
	std::mutex* l_DeviceContextMutex;
	l_DeviceContextMutex = &(UABEngine.GetMutexManager()->g_DeviceContextMutex);
	if (renderTarget)
	{
		for (int i = 0; i < m_NumViews; ++i)
		{
			l_DeviceContextMutex->lock();
			GetDeviceContext()->ClearRenderTargetView(m_CurrentRenderTargetViews[i], &backgroundColor.x);
			l_DeviceContextMutex->unlock();
		}	
	}
	if (depthStencil)
	{
		l_DeviceContextMutex->lock();
		GetDeviceContext()->ClearDepthStencilView(m_CurrentDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		l_DeviceContextMutex->unlock();
	}
}

void CContextManager::SetWorldMatrix(const Mat44f& _Model)
{
	CEffectManager::m_SceneParameters.m_World = _Model;
}

void CContextManager::SetCamera(const Mat44f& _View, const Mat44f& _Projection)
{
	CEffectManager::m_SceneParameters.m_View = _View;
	CEffectManager::m_SceneParameters.m_Projection = _Projection;
}
void CContextManager::SetCamera(const CCamera& _Camera)
{
	CEffectManager::m_SceneParameters.m_View = _Camera.GetView();
	CEffectManager::m_SceneParameters.m_Projection = _Camera.GetProjection();
	CEffectManager::m_SceneParameters.m_CameraPosition = _Camera.GetPosition();
	CEffectManager::m_SceneParameters.m_CameraProjectionInfo = Vect4f(
		_Camera.GetZNear(),
		_Camera.GetZFar(), 
		_Camera.GetFOV(), 
		_Camera.GetAspectRatio()
		);
}


ID3D11RenderTargetView* const* CContextManager::GetCurrentRenderTargetViews()
{
	return m_CurrentRenderTargetViews;
}
ID3D11DepthStencilView* CContextManager::GetCurrentDepthStencilView()
{
	return m_CurrentDepthStencilView;
}
ID3D11DepthStencilView* CContextManager::GetDepthStencilView()
{
	return m_DepthStencilView;
}
IDXGISwapChain*	CContextManager::GetSwapChain()
{
	return m_SwapChain;
}