#include "Texture.h"
#include "ContextManager\ContextManager.h"
#include "Engine\UABEngine.h"

#include <D3DX11tex.h>

CTexture::CTexture(): CNamed(""),
	m_Texture(nullptr),
	m_SamplerState(nullptr)
{
}


CTexture::~CTexture(void)
{
}

bool CTexture::LoadFile()
{
	ID3D11Device *l_Device=UABEngine.GetRenderManager()->GetDevice();
	HRESULT l_HR=D3DX11CreateShaderResourceViewFromFile(l_Device,m_Name.c_str(), NULL, NULL, &m_Texture, NULL );
	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
	l_SampDesc.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_SampDesc.AddressU=D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.AddressV=D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.AddressW=D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.ComparisonFunc=D3D11_COMPARISON_NEVER;
	l_SampDesc.MinLOD=0;
	l_SampDesc.MaxLOD=D3D11_FLOAT32_MAX;
	l_HR=l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);
	return !FAILED(l_HR);
}

void CTexture::Unload()
{

}

bool CTexture::Load(const std::string &Filename)
{
	SetName(Filename);
	return LoadFile();
}
void CTexture::Activate(unsigned int StageId)
{
	ID3D11DeviceContext *l_DeviceContext=UABEngine.GetRenderManager()->GetDeviceContext();
	l_DeviceContext->PSSetSamplers(StageId, 1, &m_SamplerState);
	l_DeviceContext->PSSetShaderResources(StageId, 1, &m_Texture);
}
bool CTexture::Reload()
{
	Unload();
	return LoadFile();
}