#include "CapturedFrameBufferTexture.h"

#include "Engine\UABEngine.h"
#include "XML\XMLTreeNode.h"

CCapturedFrameBufferTexture::CCapturedFrameBufferTexture(const CXMLTreeNode &_TreeNode)
{
	m_Name = _TreeNode.GetPszProperty("name");
	bool l_TextuerWidthAsFrameBuffer = _TreeNode.GetBoolProperty("texture_width_as_frame_buffer");
	if (l_TextuerWidthAsFrameBuffer)
	{
		m_Width = UABEngine.GetRenderManager()->GetContextManager()->GetWidth();
		m_Height = UABEngine.GetRenderManager()->GetContextManager()->GetHeight();
	}
	else
	{
		m_Width = _TreeNode.GetIntProperty("width");
		m_Height = _TreeNode.GetIntProperty("height");		
	}	
	Init(m_Name,m_Width,m_Height);
}

CCapturedFrameBufferTexture::CCapturedFrameBufferTexture(const std::string &_Name, unsigned int _Width, unsigned int _Height)
{
	Init(_Name,_Width,_Height);
}

CCapturedFrameBufferTexture::~CCapturedFrameBufferTexture()
{
}

void CCapturedFrameBufferTexture::Init(const std::string &_Name, unsigned int _Width, unsigned int _Height)
{
	m_Width = _Width;
	m_Height = _Height;
	m_Name = _Name;

	CRenderManager &l_RenderManager = *(UABEngine.GetRenderManager());

	ID3D11Device *l_Device = l_RenderManager.GetDevice();
	ID3D11Texture2D *l_Buffer = NULL;
	l_RenderManager.GetSwapChain()->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&l_Buffer);

	D3D11_TEXTURE2D_DESC l_texture2DDescription;
	l_texture2DDescription.Width = _Width;
	l_texture2DDescription.Height = _Height;
	l_texture2DDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	l_texture2DDescription.ArraySize = 1;
	l_texture2DDescription.MiscFlags = 0;
	l_texture2DDescription.MipLevels = 1;
	l_texture2DDescription.CPUAccessFlags = 0;
	l_texture2DDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	l_texture2DDescription.SampleDesc.Count = 1;
	l_texture2DDescription.SampleDesc.Quality=0;
	l_texture2DDescription.Usage = D3D11_USAGE_DEFAULT;

	HRESULT l_HR = l_Device->CreateTexture2D(&l_texture2DDescription, NULL, &m_DataTexture);
	if (FAILED(l_HR))
	{
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC DescRV;
	ZeroMemory(&DescRV,sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	DescRV.Format = l_texture2DDescription.Format;
	DescRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	DescRV.Texture2D.MipLevels = l_texture2DDescription.MipLevels;
	DescRV.Texture2D.MostDetailedMip = 0;

	l_HR = l_Device->CreateShaderResourceView(m_DataTexture, &DescRV, &m_Texture);
	if (l_HR)
	{
		return;
	}
	CreateSamplerState();
}

void CCapturedFrameBufferTexture::Unload()
{
	CTexture::Unload();
}

bool CCapturedFrameBufferTexture::CreateSamplerState()
{
	ID3D11Device *l_Device = UABEngine.GetRenderManager()->GetDevice();
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
	return !(FAILED(l_HR));
}

bool CCapturedFrameBufferTexture::Reload()
{
	return false;
}

bool CCapturedFrameBufferTexture::Capture(unsigned int StagedId)
{
	CRenderManager &l_RenderManager = *(UABEngine.GetRenderManager());
	ID3D11Texture2D	*l_Surface = NULL;
	HRESULT l_HR = l_RenderManager.GetSwapChain()->GetBuffer(StagedId, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&l_Surface));
	if (FAILED(l_HR) || l_Surface == NULL || m_DataTexture == NULL)
	{
		return false;
	}
	l_RenderManager.GetDeviceContext()->CopyResource(m_DataTexture, l_Surface);
	return true;
}