#include "Texture.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"

#include "Utils.h"

#include <directxmath.h>
#include "DirectXTex.h"

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
	//HRESULT l_HR=D3DX11CreateShaderResourceViewFromFile(l_Device, m_Name.c_str(), NULL, NULL, &m_Texture, NULL );

	wchar_t* wName = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, m_Name.c_str(), -1, wName, 4096);

	DirectX::TexMetadata mdata;
	HRESULT l_HR = DirectX::GetMetadataFromDDSFile(wName, DirectX::DDS_FLAGS_NONE, mdata);
	if (FAILED(l_HR))
	{
		HRESULT l_HR = DirectX::GetMetadataFromTGAFile(wName, mdata);
		if (FAILED(l_HR))
		{
			HRESULT l_HR = DirectX::GetMetadataFromWICFile(wName, DirectX::WIC_FLAGS_NONE, mdata);
			if (FAILED(l_HR))
			{
				MessageBox(NULL, "Could not load texture!", "EROR", MB_OK | MB_ICONEXCLAMATION);
				return 0;

			}else{
				// WIC Texture
				DirectX::ScratchImage image;
				l_HR = DirectX::LoadFromWICFile(wName, DirectX::WIC_FLAGS_NONE, &mdata, image);
				if (FAILED(l_HR))
				{
					MessageBox(NULL, "Failed to load WIC texture file!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
					return 0;
				}

				l_HR = CreateShaderResourceView(l_Device, image.GetImages(), image.GetImageCount(), mdata, &m_Texture);
				if (FAILED(l_HR))
				{
					MessageBox(NULL, "Failed creating WIC texture from file!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
					return 0;
				}
			}
		}else{
			// TGA Texture
			DirectX::ScratchImage image;
			l_HR = DirectX::LoadFromTGAFile(wName, &mdata, image);
			if (FAILED(l_HR))
			{
				MessageBox(NULL, "Failed to load TGA texture file!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}

			l_HR = CreateShaderResourceView(l_Device, image.GetImages(), image.GetImageCount(), mdata, &m_Texture);
			if (FAILED(l_HR))
			{
				MessageBox(NULL, "Failed creating TGA texture from file!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
		}
	}else{
		// DDS Texture
		if (mdata.dimension == DirectX::TEX_DIMENSION_TEXTURE3D)
		{
			if (mdata.arraySize > 1)
			{
				MessageBox(NULL, "Arrays of volume textures are not supported!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
			m_iMaxIndex = static_cast<size_t>(mdata.depth);
		}
		else{
			m_iMaxIndex = static_cast<size_t>(mdata.arraySize);
		}

		switch (mdata.format)
		{
		case DXGI_FORMAT_BC6H_TYPELESS:
		case DXGI_FORMAT_BC6H_UF16:
		case DXGI_FORMAT_BC6H_SF16:
		case DXGI_FORMAT_BC7_TYPELESS:
		case DXGI_FORMAT_BC7_UNORM:
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			if (l_Device->GetFeatureLevel() < D3D_FEATURE_LEVEL_11_0)
			{
				MessageBox(NULL, "BC6H/BC7 requires DirectX 11 hardware", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
			break;

		default:
		{
			UINT flags = 0;
			l_HR = l_Device->CheckFormatSupport(mdata.format, &flags);
			if (FAILED(l_HR) || !(flags & (D3D11_FORMAT_SUPPORT_TEXTURE1D | D3D11_FORMAT_SUPPORT_TEXTURE2D | D3D11_FORMAT_SUPPORT_TEXTURE3D)))
			{
				MessageBox(NULL, "Format not supported by DirectX hardware", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
		}
		break;
		}

		DirectX::ScratchImage image;
		l_HR = DirectX::LoadFromDDSFile(wName, DirectX::DDS_FLAGS_NONE, &mdata, image);
		if (FAILED(l_HR))
		{
			MessageBox(NULL, "Failed to load DDS texture file!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}

		// Special case to make sure Texture cubes remain arrays
		mdata.miscFlags &= ~DirectX::TEX_MISC_TEXTURECUBE;

		l_HR = DirectX::CreateShaderResourceView(l_Device, image.GetImages(), image.GetImageCount(), mdata, &m_Texture);
		if (FAILED(l_HR))
		{
			MessageBox(NULL, "Failed creating DDS texture from file!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
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
	for (int i = 0; i < _NTabs; i++)
	{
		fprintf_s(_File, "\t");
	}
	fprintf_s(_File,"<texture type=\"%s\" filename=\"%s\"/>\n",m_Type.c_str(),m_Name.c_str());
}