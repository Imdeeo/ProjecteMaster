#pragma once

#include <d3d11.h>

#include "Utils.h"
#include "Math\Matrix44.h"
#include "Math\Vector4.h"
#include "Math\Color.h"

class CEffect
{
public:

	CEffect()
		: m_VertexShader(nullptr)
		, m_PixelShader(nullptr)
		, m_VertexLayout(nullptr)
		, m_ConstantBuffer(nullptr)
	{}

	~CEffect()
	{
		CHECKED_RELEASE(m_VertexShader);
		CHECKED_RELEASE(m_PixelShader);
		CHECKED_RELEASE(m_VertexLayout);
		CHECKED_RELEASE(m_ConstantBuffer);
	}

	void UpdateParameters(ID3D11DeviceContext *Device, void* _Parameters)
	{
		Device->UpdateSubresource(m_ConstantBuffer, 0, NULL, _Parameters, 0, 0);
	}

	void SetActive(ID3D11DeviceContext *Device)
	{
		Device->IASetInputLayout(m_VertexLayout);
		Device->VSSetShader(m_VertexShader, NULL, 0);
		Device->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
		Device->PSSetShader(m_PixelShader, NULL, 0);
	}

protected:
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_VertexLayout;
	ID3D11Buffer * m_ConstantBuffer;
};



class CEffectParameters
{
public:
	Mat44f				m_World;
	Mat44f				m_View;
	Mat44f				m_Projection;
	CColor				m_BaseColor;
	Vect4f				m_CameraRightVector;
	Vect4f				m_CameraUpVector;
	float				m_DebugRenderScale;

	uint8_t				offset[9];
};
