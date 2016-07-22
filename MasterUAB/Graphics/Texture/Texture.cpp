#include "Texture.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"

#include "Utils.h"

#include <directxmath.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <fstream>
#include <utility>
#include <string>

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
	ID3D11DeviceContext *l_Context = UABEngine.GetRenderManager()->GetDeviceContext();
	std::wstring wName;
	wName.assign(m_Name.begin(), m_Name.end());

	// DirectXTK
	HRESULT l_HR = DirectX::CreateDDSTextureFromFile(l_Device, l_Context, wName.c_str(), nullptr, &m_Texture);
	if (FAILED(l_HR))
	{
		l_HR = DirectX::CreateWICTextureFromFile(l_Device, l_Context, wName.c_str(), nullptr, &m_Texture);
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

bool CTexture::LoadBuffer(unsigned char* _Buffer, size_t _Size)
{
	ID3D11Device *l_Device = UABEngine.GetRenderManager()->GetDevice();
	ID3D11DeviceContext *l_Context = UABEngine.GetRenderManager()->GetDeviceContext();
	/*std::wstring wName;
	wName.assign(m_Name.begin(), m_Name.end());*/

	// DirectXTK

	//HRESULT l_HR = DirectX::CreateWICTextureFromMemory(l_Device, _Buffer, _Size, nullptr, &m_Texture);
	//std::basic_ifstream<unsigned char> file("image.png", std::ios::binary);
	//HRESULT l_HR;
	/*l_HR = false;
	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		int length = file.tellg();
		file.seekg(0, std::ios::beg);

		unsigned char* buffer = new unsigned char[length];
		file.read(&buffer[0], length);
		file.close();

		int l_length = strlen((char*)_Buffer)/8;
		l_HR = DirectX::CreateWICTextureFromMemory(l_Device, l_Context, &_Buffer[0], l_length, nullptr, &m_Texture, NULL);
		//l_HR = DirectX::CreateWICTextureFromMemory(l_Device, l_Context, &buffer[0], length, nullptr, &m_Texture, NULL);
	}*/
	
	
	HRESULT l_HR = DirectX::CreateDDSTextureFromMemory(l_Device, (const uint8_t*)_Buffer, _Size, nullptr, &m_Texture, NULL);
	
	//HRESULT l_HR = DirectX::CreateWICTextureFromMemory(l_Device, l_Context, (const uint8_t*)&_Buffer[0], (size_t)l_length, nullptr, &m_Texture, NULL);




	//(l_Device, l_Context, wName.c_str(), nullptr, &m_Texture);
	if (FAILED(l_HR))
	{
		//l_HR = DirectX::CreateWICTextureFromFile(l_Device, l_Context, wName.c_str(), nullptr, &m_Texture);
		//if (FAILED(l_HR))
		//{
			InfoMessage("Error loading file %s of type %d", m_Name.c_str(), l_HR);
			return 0;

		//}
	}

	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
	l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	l_SampDesc.MinLOD = 0;
	l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	l_HR = l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);
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