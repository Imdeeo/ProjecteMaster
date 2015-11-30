#ifndef RENDERABLE_VERTEX_H
#define RENDERABLE_VERTEX_H

#include "RenderManager.h"
#include <assert.h>

class CEffectTechnique;

class CRenderableVertexs
{
public:
	virtual bool Render(CRenderManager *RenderManager, CEffectTechnique	*EffectTechnique, void *Parameters)
	{
		assert(!"This method mustn't be called");
		return false;
	}
	virtual bool RenderIndexed(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique,
		void *Parameters, unsigned int IndexCount=-1, unsigned int StartIndexLocation=0,
		unsigned int BaseVertexLocation=0)
	{
		assert(!"This method mustn't be called");
		return false;
	}
};

template<class T>
class CTemplatedRenderableVertexs : public CRenderableVertexs
{
private:
	ID3D11Buffer *m_VertexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	unsigned int m_VertexsCount;
	unsigned int m_PrimitiveCount;
public:
	CTemplatedRenderableVertexs(void *Vtxs, unsigned int VtxsCount, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, unsigned int PrimitiveCount)
		: m_VertexsCount(VtxsCount)
		, m_PrimitiveTopology(PrimitiveTopology)
		, m_PrimitiveCount(PrimitiveCount)
	{
		D3D11_BUFFER_DESC l_BufferDescription;
		ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
		l_BufferDescription.Usage=D3D11_USAGE_DEFAULT;
		l_BufferDescription.ByteWidth=sizeof(T)*m_VertexsCount;
		l_BufferDescription.BindFlags=D3D11_BIND_VERTEX_BUFFER;
		l_BufferDescription.CPUAccessFlags=0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = Vtxs;
		ID3D11Device *l_Device=UABEngine.GetRenderManager().GetDevice();
		HRESULT l_HR=l_Device->CreateBuffer(&l_BufferDescription, &InitData, &m_VertexBuffer);
		if(FAILED(l_HR))
			return;
	}
	virtual ~CTemplatedRenderableVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
	}
	bool Render(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique, void *Parameters)
	{
		CEffectVertexShader *l_EffectVertexShader=EffectTechnique->GetVertexShader();
		CEffectPixelShader *l_EffectPixelShader=EffectTechnique->GetPixelShader();
		if(l_EffectPixelShader==NULL || l_EffectVertexShader==NULL)
			return false;
		ID3D11DeviceContext *l_DeviceContext=RenderManager->GetDeviceContext();
		UINT stride=sizeof(T);
		UINT offset=0;
		l_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		l_DeviceContext->IASetPrimitiveTopology(m_PrimitiveTopology);
		l_DeviceContext->IASetInputLayout(l_EffectVertexShader->GetVertexLayout());
		l_DeviceContext->VSSetShader(l_EffectVertexShader->GetVertexShader(), NULL, 0);
		ID3D11Buffer *l_ConstantBufferVS=l_EffectVertexShader->GetConstantBuffer();
		l_DeviceContext->UpdateSubresource(l_ConstantBufferVS, 0, NULL, Parameters, 0, 0 );
		l_DeviceContext->VSSetConstantBuffers( 0, 1, &l_ConstantBufferVS);
		l_DeviceContext->PSSetShader(l_EffectPixelShader->GetPixelShader(), NULL, 0);
		ID3D11Buffer *l_ConstantBufferPS=l_EffectPixelShader->GetConstantBuffer();
		l_DeviceContext->PSSetConstantBuffers(0, 1, &l_ConstantBufferPS);
		l_DeviceContext->Draw(m_VertexsCount, 0);
		return true;
	}
};
#define CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(ClassName, TopologyType) \
	template<class T> \
class ClassName : public CTemplatedRenderableVertexs<T> \
{ \
public: \
	ClassName(void *Vtxs, unsigned int VtxsCount, unsigned int PrimitiveCount) \
	: CTemplatedRenderableVertexs(Vtxs, VtxsCount, TopologyType, PrimitiveCount) \
{ \
} \
};
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CLinesListRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CTrianglesListRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CTrianglesStripRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

template<class T>
class CTemplatedRenderableIndexedVertexs : public CRenderableVertexs
{
private:
	ID3D11Buffer *m_VertexBuffer;
	ID3D11Buffer *m_IndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	DXGI_FORMAT m_IndexType;
	unsigned int m_VertexsCount;
	unsigned int m_IndexsCount;
public:
	CTemplatedRenderableIndexedVertexs(void *Vtxs, unsigned int VtxsCount, void *Indices, unsigned int IndexsCount, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, DXGI_FORMAT IndexType)
		: m_VertexsCount(VtxsCount)
		, m_IndexsCount(IndexsCount)
		, m_PrimitiveTopology(PrimitiveTopology)
		, m_VertexBuffer(0)
		, m_IndexBuffer(0)
		, m_IndexType(IndexType)
	{
		D3D11_BUFFER_DESC l_VertexBufferDesc;
		ZeroMemory(&l_VertexBufferDesc, sizeof(l_VertexBufferDesc));
		l_VertexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
		l_VertexBufferDesc.ByteWidth=sizeof(T)*m_VertexsCount;
		l_VertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
		l_VertexBufferDesc.CPUAccessFlags=0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem=Vtxs;
		ID3D11Device *l_Device=UABEngine.GetRenderManager().GetDevice();
		HRESULT hr=l_Device->CreateBuffer(&l_VertexBufferDesc, &InitData, &m_VertexBuffer);
		if(FAILED(hr))
			return;
		D3D11_BUFFER_DESC l_IndexBuffer;
		ZeroMemory(&l_IndexBuffer, sizeof(l_IndexBuffer));
		l_IndexBuffer.Usage=D3D11_USAGE_DEFAULT;
		l_IndexBuffer.ByteWidth=(m_IndexType==DXGI_FORMAT_R16_UINT ?sizeof(WORD) : sizeof(unsigned int))*m_IndexsCount;
		l_IndexBuffer.BindFlags=D3D11_BIND_INDEX_BUFFER;
		l_IndexBuffer.CPUAccessFlags=0;
		InitData.pSysMem=Indices;
		hr=l_Device->CreateBuffer(&l_IndexBuffer, &InitData, &m_IndexBuffer);
		if(FAILED(hr))
			return;
	}
	virtual ~CTemplatedRenderableIndexedVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
		CHECKED_RELEASE(m_IndexBuffer);
	}
	bool RenderIndexed(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique, void *Parameters, unsigned int IndexCount=-1, unsigned int StartIndexLocation=0, unsigned int BaseVertexLocation=0)
	{
		if(EffectTechnique==NULL)
			return false;
		CEffectVertexShader *l_EffectVertexShader=EffectTechnique->GetVertexShader();
		CEffectPixelShader *l_EffectPixelShader=EffectTechnique->GetPixelShader();
		ID3D11Buffer *l_ConstantBufferVS=l_EffectVertexShader->GetConstantBuffer();
		if(l_EffectPixelShader==NULL || l_EffectVertexShader==NULL || l_ConstantBufferVS==NULL)
			return false;
		ID3D11DeviceContext *l_DeviceContext=RenderManager->GetDeviceContext();
		UINT stride=sizeof(T);
		UINT offset=0;
		l_DeviceContext->IASetIndexBuffer(m_IndexBuffer, m_IndexType, 0);
		l_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride,	&offset);
		l_DeviceContext->IASetPrimitiveTopology(m_PrimitiveTopology);
		l_DeviceContext->IASetInputLayout(l_EffectVertexShader->GetVertexLayout());
		l_DeviceContext->VSSetShader(l_EffectVertexShader->GetVertexShader(), NULL, 0);
		l_DeviceContext->UpdateSubresource(l_ConstantBufferVS, 0, NULL, Parameters, 0, 0 );
		l_DeviceContext->VSSetConstantBuffers(0, 1, &l_ConstantBufferVS);
		l_DeviceContext->PSSetShader(l_EffectPixelShader->GetPixelShader(), NULL, 0);
		ID3D11Buffer *l_ConstantBufferPS=l_EffectPixelShader->GetConstantBuffer();
		l_DeviceContext->PSSetConstantBuffers(0, 1, &l_ConstantBufferPS);
		l_DeviceContext->DrawIndexed(IndexCount==-1 ? m_IndexsCount : IndexCount, StartIndexLocation, BaseVertexLocation);
		return true;
	}
};
#define CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(ClassName, TopologyType, IndexType) \
	template<class T> \
class ClassName : public CTemplatedRenderableIndexedVertexs<T> \
{ \
public: \
		ClassName(void *Vtxs, unsigned int VtxsCount, void *Indices, unsigned int IndexsCount) \
		: CTemplatedRenderableIndexedVertexs(Vtxs, VtxsCount, Indices, IndexsCount,	TopologyType, IndexType) \
{ \
} \
};
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABTriangleListRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABTriangleListRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R32_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABTriangleStripRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABTriangleStripRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R32_UINT);

#endif //RENDERABLE_VERTEX_H


