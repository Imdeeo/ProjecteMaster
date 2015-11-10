#include "ContextManagerD3D.h"

#include <Windows.h>
#include <d3d11.h>
//#include <d3dx11.h>

#pragma comment(lib,"d3d11.lib")

CContextManagerD3D *CContextManagerD3D::m_Instance = NULL;

CContextManagerD3D::CContextManagerD3D(void):
	m_D3DDevice(NULL),
	m_DeviceContext(NULL),
	m_SwapChain(NULL)
{
}


CContextManagerD3D::~CContextManagerD3D(void)
{
	m_D3DDevice->Release();
	m_DeviceContext->Release();
	m_SwapChain->Release();

}


void CContextManagerD3D::Render()
{
	float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
			+0.0f, +0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
			+0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,
		};

	UINT indices[] = {
        0, 1, 2 // front face
    };

	ID3D11Buffer* l_VertexBuffer;
	ID3D11Buffer* l_IndexBuffer;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage=D3D11_USAGE_DEFAULT;
	desc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	// TODO
	// desc.ByteWidth;
	// desc.CPUAccessFlags;
	desc.ByteWidth = 5;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem=vertices;
	m_D3DDevice->CreateBuffer(&desc,&InitData,&l_VertexBuffer);

	D3D11_SUBRESOURCE_DATA InitData2 = {};
	D3D11_BUFFER_DESC desc2 = {};
	desc2.Usage=D3D11_USAGE_DEFAULT;
	desc2.BindFlags=D3D11_BIND_INDEX_BUFFER;
	desc2.ByteWidth = 5;
	desc2.CPUAccessFlags = 0;
	// ID3D11DeviceContext::CreateBuffer
	m_D3DDevice->CreateBuffer(&desc2,&InitData2,&l_IndexBuffer);


}

bool CContextManagerD3D::InitDevice(HWND OutputWindowInstance, UINT Width, UINT Height)
{
	// Tendremos que crear y rellenar una estructura de este tipo
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	// o
	//DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Windowed = TRUE;
	// TODO:
	desc.BufferDesc.Width = Width;
	desc.BufferDesc.Height = Height;
	desc.BufferDesc.RefreshRate.Numerator = 1;
	desc.BufferDesc.RefreshRate.Denominator = 60;
	desc.OutputWindow = OutputWindowInstance;
	desc.SampleDesc.Count = 1;
	//desc.SampleDesc.Quality;
	//desc. ????
	
	// Que DirectX queremos
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &desc, &m_SwapChain, &m_D3DDevice, NULL, &m_DeviceContext)))
	{
		return S_FALSE;
	}
	return S_OK;
}

void CContextManagerD3D::Draw()
{

	
	// Limpiar el framebuffer:
	// ID3D11DeviceContext::ClearRenderTargetView
	// ID3D11DeviceContext::ClearDepthStencilView
	
	const FLOAT color[4] = {1.f,0.f,1.f,1.f};
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView,color);
	//m_DeviceContext->ClearDepthStencilView()

	// definir a que target vamos a pintar:
	// ID3D11DeviceContext::OMSetRenderTargets
	m_DeviceContext->OMSetRenderTargets(1,&m_RenderTargetView,nullptr);

	// definir el viewport:
	// ID3D11DeviceContext::RSSetViewports
	D3D11_VIEWPORT l_viewport = {0,0,800,600,0,1};
	m_DeviceContext->RSSetViewports(1,&l_viewport);

	Render();

	// presentar lo pintado:
	// IDXGISwapChain::Present
	m_SwapChain->Present(0,0);
}

HRESULT CContextManagerD3D::CreateRenderTargetView()
{
	

	ID3D11Texture2D *pBackBuffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();
	return hr;
		
}

//
//HRESULT CContextManagerD3D::CreateDepthStencil()
//{
//	D3D11_TEXTURE2D_DESC desc = {};
//	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//	// TODO:
//	//desc.Width;
//	//desc.Height;
//	//desc.MipLevels;
//	//desc.ArraySize;
//	//desc.SampleDesc.Count;
//	//desc.SampleDesc.Quality;
//	HRESULT hr = m_D3DDevice->CreateTexture2D(&desc, NULL, &m_DepthStencil);
//	if (FAILED(hr))
//		return hr;
//}
//
//HRESULT CContextManagerD3D::CreateDepthStencilView()
//{
//	D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
//	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//	desc.Texture2D.MipSlice = 0;
//	// TODO
//	//desc.Format;
//	HRESULT hr = m_D3DDevice->CreateDepthStencilView(m_DepthStencil, &desc, &m_DepthStencilView);
//	if (FAILED(hr))
//		return hr;
//}
