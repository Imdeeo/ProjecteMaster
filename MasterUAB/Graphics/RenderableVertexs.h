#pragma once

#include <D3D11.h>

#include <assert.h>

#include "Utils.h"

class CRenderableVertexs
{
protected:
	ID3D11Buffer*					m_VertexBuffer;
	ID3D11Buffer*					m_IndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY		m_PrimitiveTopology;
	DXGI_FORMAT						m_IndexType;
	unsigned int					m_VertexsCount;
	unsigned int					m_IndexsCount;
	unsigned int					m_PrimitiveCount;
	unsigned int					m_Stride;
public:

	CRenderableVertexs()
		: m_VertexBuffer(nullptr)
		, m_IndexBuffer(nullptr)
	{}

	~CRenderableVertexs()
	{
		CHECKED_DELETE(m_VertexBuffer);
		CHECKED_DELETE(m_IndexBuffer);
	}

	void SetBuffers(ID3D11DeviceContext *Device)
	{
		UINT offset = 0;
		Device->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_Stride, &offset);
		Device->IASetPrimitiveTopology(m_PrimitiveTopology);

		if (m_IndexBuffer != nullptr)
		{
			Device->IASetIndexBuffer(m_IndexBuffer, m_IndexType, 0);
		}
	}

	void Draw(ID3D11DeviceContext *Device, unsigned int IndexCount = -1, unsigned int StartIndexLocation = 0, unsigned int BaseVertexLocation = 0)
	{
		if (m_IndexBuffer == nullptr)
			Device->Draw(m_VertexsCount, 0);
		else
			Device->DrawIndexed(IndexCount == -1 ? m_IndexsCount : IndexCount, StartIndexLocation, BaseVertexLocation);
	}
};



// ************************************************************************************************************************************************************************************




template<class T>
class CTemplatedRenderableVertexs : public CRenderableVertexs
{
public:
	CTemplatedRenderableVertexs(ID3D11Device *Device, void *Vtxs, unsigned int VtxsCount, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, unsigned int PrimitiveCount)
	{
		m_VertexsCount = VtxsCount;
		m_PrimitiveTopology = PrimitiveTopology;
		m_PrimitiveCount = PrimitiveCount;
		m_Stride = sizeof(T);

		D3D11_BUFFER_DESC l_BufferDescription;
		ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
		l_BufferDescription.Usage = D3D11_USAGE_DEFAULT;
		l_BufferDescription.ByteWidth = sizeof(T)*m_VertexsCount;
		l_BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		l_BufferDescription.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = Vtxs;
		HRESULT l_HR = Device->CreateBuffer(&l_BufferDescription, &InitData, &m_VertexBuffer);
		if (FAILED(l_HR))
			return;
	}
};

#define CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(ClassName, TopologyType) \
template<class T> \
class ClassName : public CTemplatedRenderableVertexs<T> \
{ \
public: \
	ClassName(ID3D11Device* Device, void *Vtxs, unsigned int VtxsCount, unsigned int PrimitiveCount) \
	: CTemplatedRenderableVertexs(Device, Vtxs, VtxsCount, TopologyType, PrimitiveCount) \
	{ \
	} \
};

CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CLinesListRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CTrianglesListRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CTrianglesStripRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

template<class T>
class CTemplatedRenderableIndexedVertexs : public CRenderableVertexs
{
public:
	CTemplatedRenderableIndexedVertexs(ID3D11Device *Device, void *Vtxs, unsigned int VtxsCount, void *Indices, unsigned int IndexsCount, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, DXGI_FORMAT IndexType)	
	{
		m_VertexsCount = VtxsCount;
		m_IndexsCount = IndexsCount;
		m_PrimitiveTopology = PrimitiveTopology;
		m_IndexType = IndexType;
		m_Stride = sizeof(T);

		D3D11_BUFFER_DESC l_VertexBufferDesc;
		ZeroMemory(&l_VertexBufferDesc, sizeof(l_VertexBufferDesc));
		l_VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		l_VertexBufferDesc.ByteWidth = sizeof(T)*m_VertexsCount;
		l_VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		l_VertexBufferDesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = Vtxs;
		HRESULT hr = Device->CreateBuffer(&l_VertexBufferDesc, &InitData, &m_VertexBuffer);
		if (FAILED(hr))
			return;

		D3D11_BUFFER_DESC l_IndexBufferDesc;
		ZeroMemory(&l_IndexBufferDesc, sizeof(l_IndexBufferDesc));
		l_IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		l_IndexBufferDesc.ByteWidth = (m_IndexType == DXGI_FORMAT_R16_UINT ? sizeof(WORD) : sizeof(unsigned int))*m_IndexsCount;
		l_IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		l_IndexBufferDesc.CPUAccessFlags = 0;
		InitData.pSysMem = Indices;
		hr = Device->CreateBuffer(&l_IndexBufferDesc, &InitData, &m_IndexBuffer);
		if (FAILED(hr))
			return;
	}
};

#define CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(ClassName, TopologyType, IndexType) \
template<class T> \
class ClassName : public CTemplatedRenderableIndexedVertexs<T> \
{ \
public: \
	ClassName(ID3D11Device* Device, void *Vtxs, unsigned int VtxsCount, void *Indices, unsigned int IndexsCount) \
	: CTemplatedRenderableIndexedVertexs(Device, Vtxs, VtxsCount, Indices, IndexsCount, TopologyType, IndexType) \
	{ \
	} \
};

CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CTriangleListRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CTriangleListRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R32_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CTriangleStripRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CTriangleStripRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R32_UINT);

CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CLinesListRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, DXGI_FORMAT_R16_UINT);
