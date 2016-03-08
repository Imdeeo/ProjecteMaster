#include "DynamicTexture.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"

#include "XML\XMLTreeNode.h"
#include "Utils\Named.h"

#include <d3d11.h>

#include <assert.h>

CDynamicTexture::CDynamicTexture(const std::string &Name, int Width, int Height, bool CreateDepthStencilBuffer):
m_Width(Width), m_Height(Height), m_CreateDepthStencilBuffer(CreateDepthStencilBuffer), m_DepthStencilBuffer(nullptr), m_DepthStencilView(nullptr)
{
	SetName(Name);
	Init();
}

CDynamicTexture::CDynamicTexture(const CXMLTreeNode &TreeNode) : m_DepthStencilBuffer(nullptr), m_DepthStencilView(nullptr)
{
	//<dynamic_texture name = "DepthMapTexture" texture_width_as_frame_buffer = "true" format_type = "R32F" / >
	
	std::string l_name = TreeNode.GetPszProperty("name");
	SetName(l_name);
	if (TreeNode.GetBoolProperty("texture_width_as_frame_buffer"))
	{
		m_Width = UABEngine.GetRenderManager()->GetContextManager()->GetWidth();
		m_Height = UABEngine.GetRenderManager()->GetContextManager()->GetHeight();
	}
	else
	{
		m_Width = TreeNode.GetIntProperty("width");
		m_Height = TreeNode.GetIntProperty("height");		
	}
	m_CreateDepthStencilBuffer = TreeNode.GetBoolProperty("create_depth_stencil_buffer",false);	
	Init();
}



CDynamicTexture::~CDynamicTexture()
{
}

void CDynamicTexture::Init()
{
	ID3D11Device *l_Device = UABEngine.GetRenderManager()->GetContextManager()->GetDevice();
	D3D11_TEXTURE2D_DESC l_textureDescription;

	ZeroMemory(&l_textureDescription, sizeof(D3D11_TEXTURE2D_DESC));
	l_textureDescription.Width = m_Width;
	l_textureDescription.Height = m_Height;
	l_textureDescription.MipLevels = 1;
	l_textureDescription.ArraySize = 1;
	l_textureDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //??
	l_textureDescription.SampleDesc.Count = 1;
	l_textureDescription.Usage = D3D11_USAGE_DEFAULT;
	l_textureDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	l_textureDescription.CPUAccessFlags = 0;
	l_textureDescription.MiscFlags = 0;
	HRESULT l_HR = l_Device->CreateTexture2D(&l_textureDescription, NULL, &m_RenderTargetTexture);
	assert(!FAILED(l_HR));

	D3D11_RENDER_TARGET_VIEW_DESC l_RenderTargetViewDescription;
	l_RenderTargetViewDescription.Format = l_textureDescription.Format;
	l_RenderTargetViewDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	l_RenderTargetViewDescription.Texture2D.MipSlice = 0;

	l_HR = l_Device->CreateRenderTargetView(m_RenderTargetTexture, &l_RenderTargetViewDescription, &m_RenderTargetView);
	assert(!FAILED(l_HR));

	D3D11_SHADER_RESOURCE_VIEW_DESC l_ShaderResourceViewDescription;
	l_ShaderResourceViewDescription.Format = l_textureDescription.Format;
	l_ShaderResourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	l_ShaderResourceViewDescription.Texture2D.MostDetailedMip = 0;
	l_ShaderResourceViewDescription.Texture2D.MipLevels = 1;

	l_HR = l_Device->CreateShaderResourceView(m_RenderTargetTexture, &l_ShaderResourceViewDescription, &m_Texture);
	assert(!FAILED(l_HR));

	if (m_CreateDepthStencilBuffer)
	{
		D3D11_TEXTURE2D_DESC l_DepthBufferDescription;
		ZeroMemory(&l_DepthBufferDescription, sizeof(D3D11_TEXTURE2D_DESC));

		l_DepthBufferDescription.Width = m_Width;
		l_DepthBufferDescription.Height = m_Height;
		l_DepthBufferDescription.MipLevels = 1;
		l_DepthBufferDescription.ArraySize = 1;
		l_DepthBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		l_DepthBufferDescription.SampleDesc.Count = 1;
		l_DepthBufferDescription.SampleDesc.Quality = 0;
		l_DepthBufferDescription.Usage = D3D11_USAGE_DEFAULT;
		l_DepthBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		l_DepthBufferDescription.CPUAccessFlags = 0;
		l_DepthBufferDescription.MiscFlags = 0;

		l_HR = l_Device->CreateTexture2D(&l_DepthBufferDescription, NULL, &m_DepthStencilBuffer);
		assert(!FAILED(l_HR));

		D3D11_DEPTH_STENCIL_VIEW_DESC l_DepthStencilViewDescription;
		ZeroMemory(&l_DepthStencilViewDescription, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		l_DepthStencilViewDescription.Format = l_DepthBufferDescription.Format;

		l_DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		l_DepthStencilViewDescription.Texture2D.MipSlice = 0;

		l_HR = l_Device->CreateDepthStencilView(m_DepthStencilBuffer, &l_DepthStencilViewDescription, &m_DepthStencilView);
		assert(!FAILED(l_HR));
	}
	else
	{
		m_DepthStencilBuffer = nullptr;
	}

	CreateSamplerState();
}

bool CDynamicTexture::CreateSamplerState()
{
	ID3D11Device *l_Device = UABEngine.GetRenderManager()->GetContextManager()->GetDevice();
	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(D3D11_SAMPLER_DESC));

	l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	l_SampDesc.MinLOD = 0;
	l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT l_HR = l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);
	assert(!FAILED(l_HR));
	return true;
}

ID3D11RenderTargetView* CDynamicTexture::GetRenderTargetView()
{
	return m_RenderTargetView;
}
ID3D11DepthStencilView* CDynamicTexture::GetDepthStencilView()
{
	return m_DepthStencilView;
}
