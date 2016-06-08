#include "Texture.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"

#include "Utils.h"

#include <directxmath.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

CTexture::CTexture(): CNamed(""),
	m_Texture(nullptr),
	m_SamplerState(nullptr),
	m_iMaxIndex(1)
{
}

CTexture::~CTexture(void)
{
}

bool CTexture::LoadFile()
{
	ID3D11Device *l_Device=UABEngine.GetRenderManager()->GetDevice();
	std::wstring wName;
	wName.assign(m_Name.begin(), m_Name.end());

	// DirectXTK
	HRESULT l_HR = DirectX::CreateDDSTextureFromFile(l_Device, wName.c_str(), nullptr, &m_Texture);
	if (FAILED(l_HR))
	{
		l_HR = DirectX::CreateWICTextureFromFile(l_Device, wName.c_str(), nullptr, &m_Texture);
		if (FAILED(l_HR))
		{
			InfoMessage("Error loading file %s of type %d", m_Name.c_str(), l_HR);
			return 0;

		}
	}

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
	CHECKED_RELEASE(m_SamplerState);
	CHECKED_RELEASE(m_Texture);
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

void CTexture::Save(FILE* _File, size_t _NTabs)
{
	for (size_t i = 0; i < _NTabs; i++)
	{
		fprintf_s(_File, "\t");
	}
	fprintf_s(_File,"<texture type=\"%s\" filename=\"%s\"/>\n",m_Type.c_str(),m_Name.c_str());
}