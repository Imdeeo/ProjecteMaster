#ifndef TEMPLATED_RENDERABLE_INDEXED_VERTEX_H
#define TEMPLATED_RENDERABLE_INDEXED_VERTEX_H

#include <d3d11.h>
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "ContextManager\ContextManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectShader.h"

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
	std::vector<Vect3f> m_Vtxs;
	void *m_Indexs;
public:
	CTemplatedRenderableIndexedVertexs(void *Vtxs, unsigned int VtxsCount, void	*Indices,
		unsigned int IndexsCount, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology,	DXGI_FORMAT IndexType)
	: m_VertexsCount(VtxsCount)
	, m_IndexsCount(IndexsCount)
	, m_PrimitiveTopology(PrimitiveTopology)
	, m_VertexBuffer(0)
	, m_IndexBuffer(0)
	, m_IndexType(IndexType)
	{
		m_Vtxs.resize(VtxsCount);
		T *data = (T *)Vtxs;
		for (size_t i = 0; i < VtxsCount; i++)
		{
			m_Vtxs[i] = *(Vect3f*)(&data[i]);
		}

		if (m_IndexType == DXGI_FORMAT_R16_UINT)
		{
			unsigned short *index_aux = (unsigned short*)malloc(sizeof(unsigned short)*IndexsCount);
			unsigned short *index_data = (unsigned short *)Indices;
			for (size_t i = 0; i < IndexsCount; i++)
			{
				index_aux[i] = index_data[i];
			}
			m_Indexs = (void*)index_aux;
		}
		else
		{
			unsigned int *index_aux = (unsigned int*)malloc(sizeof(unsigned int)*IndexsCount);
			unsigned int *index_data = (unsigned int*)Indices;
			for (size_t i = 0; i < IndexsCount; i++)
			{
				index_aux[i] = index_data[i];
			}
			m_Indexs = (void*)index_aux;
		}

		D3D11_BUFFER_DESC l_VertexBufferDesc;
		ZeroMemory(&l_VertexBufferDesc, sizeof(l_VertexBufferDesc));
		l_VertexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
		l_VertexBufferDesc.ByteWidth=sizeof(T)*m_VertexsCount;
		l_VertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
		l_VertexBufferDesc.CPUAccessFlags=0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem=Vtxs;
		ID3D11Device *l_Device =UABEngine.GetRenderManager()->GetDevice();
		std::mutex * l_DeviceMutex = &(UABEngine.GetMutexManager()->g_DeviceMutex);
		l_DeviceMutex->lock();
		HRESULT hr=l_Device->CreateBuffer(&l_VertexBufferDesc, &InitData,&m_VertexBuffer);
		l_DeviceMutex->unlock();
		if(FAILED(hr))
			return;
		D3D11_BUFFER_DESC l_IndexBuffer;
		ZeroMemory(&l_IndexBuffer, sizeof(l_IndexBuffer));
		l_IndexBuffer.Usage=D3D11_USAGE_DEFAULT;
		l_IndexBuffer.ByteWidth=(m_IndexType==DXGI_FORMAT_R16_UINT ? sizeof(WORD) : sizeof(unsigned int))*m_IndexsCount;
		l_IndexBuffer.BindFlags=D3D11_BIND_INDEX_BUFFER;
		l_IndexBuffer.CPUAccessFlags=0;
		InitData.pSysMem=Indices;
		l_DeviceMutex->lock();
		hr=l_Device->CreateBuffer(&l_IndexBuffer, &InitData, &m_IndexBuffer);
		l_DeviceMutex->unlock();
		if(FAILED(hr))
			return;
	}
	virtual ~CTemplatedRenderableIndexedVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
		CHECKED_RELEASE(m_IndexBuffer);
		CHECKED_DELETE(m_Indexs);
	}
	bool RenderIndexed(CRenderManager *RenderManager, CEffectTechnique*EffectTechnique,
		void *_Parameters, unsigned int IndexCount=-1, unsigned int	StartIndexLocation=0,
		unsigned int BaseVertexLocation=0)
	{
		if(EffectTechnique==NULL)
			return false;
		CEffectVertexShader *l_EffectVertexShader=EffectTechnique->GetVertexShader();
		CEffectPixelShader *l_EffectPixelShader=EffectTechnique->GetPixelShader();

		if(l_EffectPixelShader==NULL || l_EffectVertexShader==NULL)
			return false;
		ID3D11DeviceContext *l_DeviceContext;
		std::mutex* l_DeviceContextMutex = &(UABEngine.GetMutexManager()->g_DeviceContextMutex);
		std::mutex* l_DeviceMutex = &(UABEngine.GetMutexManager()->g_DeviceMutex);
		l_DeviceContextMutex->lock();
		l_DeviceMutex->lock();
		RenderManager->GetDevice()->GetImmediateContext(&l_DeviceContext);
		l_DeviceMutex->unlock();
		l_DeviceContextMutex->unlock();
		UINT stride=sizeof(T);
		UINT offset=0;
		l_DeviceContextMutex->lock();
		l_DeviceContext->IASetIndexBuffer(m_IndexBuffer, m_IndexType, 0);
		l_DeviceContextMutex->unlock();
		l_DeviceContextMutex->lock();
		l_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride,	&offset);
		l_DeviceContextMutex->unlock();
		l_DeviceContextMutex->lock();
		l_DeviceContext->IASetPrimitiveTopology(m_PrimitiveTopology);
		l_DeviceContextMutex->unlock();
		l_DeviceContextMutex->lock();
		l_DeviceContext->IASetInputLayout(l_EffectVertexShader->GetVertexLayout());
		l_DeviceContextMutex->unlock();
		l_DeviceContextMutex->lock();
		l_DeviceContext->VSSetShader(l_EffectVertexShader->GetVertexShader(), NULL, 0);
		l_DeviceContextMutex->unlock();

		//CContextManager* l_ContextManager = UABEngine.GetRenderManager()->GetContextManager();
		//l_DeviceContext->RSSetState(l_ContextManager->GetRasterizerState(CContextManager::RS_SOLID_BACK_CULL));
		//l_DeviceContext->OMSetDepthStencilState(l_ContextManager->GetDepthStencilState(CContextManager::DSS_DEPTH_ON), 0);
		/*Vect4f v(1, 1, 1, 1);
		l_DeviceContext->OMSetBlendState(l_ContextManager->GetBlendState(CContextManager::BLEND_CLASSIC), &v.x, 0xffffffff);*/

		CEffectManager* l_EffectManagerInstance = UABEngine.GetEffectManager();

		ID3D11Buffer *l_SceneConstantBufferVS = l_EffectVertexShader->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_LightConstantBufferVS = l_EffectVertexShader->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_AnimationConstantBufferVS = l_EffectVertexShader->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_MaterialParametersConstantBufferVS = l_EffectVertexShader->GetConstantBuffer(MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID);

		l_DeviceContextMutex->lock();
		l_DeviceContext->UpdateSubresource(l_MaterialParametersConstantBufferVS, 0, NULL, _Parameters, 0, 0);

		ID3D11Buffer* VSBuffers[4] = { l_SceneConstantBufferVS, l_LightConstantBufferVS, l_AnimationConstantBufferVS, l_MaterialParametersConstantBufferVS };
		l_DeviceContext->VSSetConstantBuffers(0, 4,VSBuffers);

		l_DeviceContext->PSSetShader(l_EffectPixelShader->GetPixelShader(), NULL, 0);

		l_DeviceContext->GSSetShader(NULL, NULL, 0);
		l_DeviceContextMutex->unlock();

		ID3D11Buffer *l_SceneConstantBufferPS = l_EffectVertexShader->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_LightConstantBufferPS = l_EffectVertexShader->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_AnimationConstantBufferPS = l_EffectVertexShader->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_MaterialParametersConstantBufferPS = l_EffectVertexShader->GetConstantBuffer(MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID);

		l_DeviceContextMutex->lock();
		l_DeviceContext->UpdateSubresource(l_MaterialParametersConstantBufferPS, 0, NULL,_Parameters, 0, 0);

		ID3D11Buffer* PSBuffers[4] = {l_SceneConstantBufferPS,l_LightConstantBufferPS,l_AnimationConstantBufferPS,l_MaterialParametersConstantBufferPS};
		l_DeviceContext->PSSetConstantBuffers(0, 4, PSBuffers);
		/*l_DeviceContext->PSSetConstantBuffers(0, 1, &l_SceneConstantBufferPS);
		l_DeviceContext->PSSetConstantBuffers(1, 1, &l_LightConstantBufferPS);
		l_DeviceContext->PSSetConstantBuffers(2, 1, &l_AnimationConstantBufferPS);*/

		l_DeviceContext->DrawIndexed(IndexCount==-1 ? m_IndexsCount :IndexCount, StartIndexLocation, BaseVertexLocation);
		l_DeviceContextMutex->unlock();
		return true;
	}

	const Vect3f* GetVertexs()const { return &m_Vtxs[0]; }
	const unsigned int GetNVertexs(){ return m_VertexsCount; }

	const unsigned int GetSizeOfVertex(){ return sizeof(T); }

	const void* GetIndexs()const { return m_Indexs; }
	const unsigned int GetNIndexs()	{ return m_IndexsCount; }
	const unsigned int GetSizeOfIndexs(){ return m_IndexType == DXGI_FORMAT_R16_UINT ? sizeof(unsigned short) : sizeof(unsigned int); }
};

#define CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(ClassName, TopologyType,IndexType) \
template<class T> \
class ClassName : public CTemplatedRenderableIndexedVertexs<T> \
{ \
public: \
ClassName(void *Vtxs, unsigned int VtxsCount, void *Indices, unsigned int IndexsCount) \
: CTemplatedRenderableIndexedVertexs(Vtxs, VtxsCount, Indices, IndexsCount, TopologyType, IndexType) \
{ \
} \
};

CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABLineListRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABLineListRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, DXGI_FORMAT_R32_UINT);

CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABTriangleListRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABTriangleListRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R32_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABTriangleStripRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CUABTriangleStripRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R32_UINT);

#endif //TEMPLATED_RENDERABLE_INDEXED_VERTEX_H