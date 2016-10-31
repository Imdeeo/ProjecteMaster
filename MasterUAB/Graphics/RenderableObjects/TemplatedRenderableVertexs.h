#ifndef TEMPLATED_RENDERABLE_VERTEX_H
#define TEMPLATED_RENDERABLE_VERTEX_H

#include <d3d11.h>
#include "Engine\UABEngine.h"
#include "MutexManager\MutexManager.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "ContextManager\ContextManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectShader.h"

template<class T>
class CTemplatedRenderableVertexs : public CRenderableVertexs
{
private:
	ID3D11Buffer *m_VertexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	unsigned int m_VertexsCount;
	unsigned int m_PrimitiveCount;
	std::vector<Vect3f> m_Vtxs;
public:
	CTemplatedRenderableVertexs(void *Vtxs, unsigned int VtxsCount,	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, unsigned int PrimitiveCount, bool Dynamic = false)
	: m_VertexsCount(VtxsCount)
	, m_PrimitiveTopology(PrimitiveTopology)
	, m_PrimitiveCount(PrimitiveCount)
	, m_VertexBuffer(0)
	{
		m_Vtxs.resize(VtxsCount);
		T *data = (T *)Vtxs;
		for (size_t i = 0; i < VtxsCount; i++)
		{
			m_Vtxs[i] = *(Vect3f*)(&data[i]);
		}

		D3D11_BUFFER_DESC l_BufferDescription;
		ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
		//l_BufferDescription.Usage=D3D11_USAGE_DEFAULT;
		l_BufferDescription.Usage = Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		l_BufferDescription.ByteWidth=sizeof(T)*m_VertexsCount;
		l_BufferDescription.BindFlags=D3D11_BIND_VERTEX_BUFFER;
		//l_BufferDescription.CPUAccessFlags=0;
		l_BufferDescription.CPUAccessFlags = Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = Vtxs;
		ID3D11Device *l_Device=UABEngine.GetRenderManager()->GetDevice();
		UABEngine.GetMutexManager()->g_DeviceMutex.lock();
		HRESULT l_HR=l_Device->CreateBuffer(&l_BufferDescription, &InitData, &m_VertexBuffer);
		UABEngine.GetMutexManager()->g_DeviceMutex.unlock();
		if(FAILED(l_HR))
			return;
	}

	virtual ~CTemplatedRenderableVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
	}

	bool Render(CRenderManager *RenderManager, CEffectTechnique	*EffectTechnique, void *_Parameters, int vertexsToRender)
	{
		CEffectVertexShader *l_EffectVertexShader = EffectTechnique->GetVertexShader();
		CEffectPixelShader *l_EffectPixelShader = EffectTechnique->GetPixelShader();
		CEffectGeometryShader *l_EffectGeometryShader = EffectTechnique->GetGeometryShader();

		int l_VertexsToRender = (vertexsToRender >= 0) ? vertexsToRender : m_VertexsCount;

		if(l_EffectPixelShader==NULL || l_EffectVertexShader==NULL)
			return false;

		ID3D11DeviceContext *l_DeviceContext=RenderManager->GetDeviceContext();
		UINT stride=sizeof(T);
		UINT offset=0;
		
		std::mutex* l_DeviceContextMutex = &(UABEngine.GetMutexManager()->g_DeviceContextMutex);
		l_DeviceContextMutex->lock();
		l_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
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

		//OJO CUIDAO! Añadido por Uri y Jonathan pq creen que faltaba!!!
		//CContextManager* l_ContextManager = UABEngine.GetRenderManager()->GetContextManager();
		//l_DeviceContext->RSSetState(l_ContextManager->GetRasterizerState(CContextManager::RS_SOLID_BACK_CULL));


		ID3D11Buffer *l_SceneConstantBufferVS=l_EffectVertexShader->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_LightConstantBufferVS=l_EffectVertexShader->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_AnimationConstantBufferVS=l_EffectVertexShader->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_MaterialParametersConstantBufferVS = l_EffectVertexShader->GetConstantBuffer(MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID);

		CEffectManager* l_EffectManagerInstance = UABEngine.GetEffectManager();
		if (l_MaterialParametersConstantBufferVS == NULL)
			return false;
		l_DeviceContextMutex->lock();
		l_DeviceContext->UpdateSubresource(l_MaterialParametersConstantBufferVS, 0, NULL, _Parameters, 0, 0);
		l_DeviceContextMutex->unlock();
		ID3D11Buffer* VSBuffers[4] = { l_SceneConstantBufferVS, l_LightConstantBufferVS, l_AnimationConstantBufferVS, l_MaterialParametersConstantBufferVS };
		l_DeviceContextMutex->lock();
		l_DeviceContext->VSSetConstantBuffers(0, 4,VSBuffers);
		l_DeviceContextMutex->unlock();

		if (l_EffectGeometryShader)
		{
			l_DeviceContextMutex->lock();
			l_DeviceContext->GSSetShader(l_EffectGeometryShader->GetGeometryShader(), NULL, 0);
			l_DeviceContextMutex->unlock();

			ID3D11Buffer *l_SceneConstantBufferGS = l_EffectGeometryShader->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
			ID3D11Buffer *l_LightConstantBufferGS = l_EffectGeometryShader->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
			ID3D11Buffer *l_AnimationConstantBufferGS = l_EffectGeometryShader->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
			ID3D11Buffer *l_MaterialParametersConstantBufferGS = l_EffectGeometryShader->GetConstantBuffer(MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID);
			ID3D11Buffer* GSBuffers[4] = { l_SceneConstantBufferGS, l_LightConstantBufferGS, l_AnimationConstantBufferGS, l_MaterialParametersConstantBufferGS };
			//ID3D11Buffer *l_ConstantBufferGS = l_EffectGeometryShader->GetConstantBuffer(0);
			
			l_DeviceContextMutex->lock();
			l_DeviceContext->UpdateSubresource(l_MaterialParametersConstantBufferGS, 0, NULL, _Parameters, 0, 0);
			l_DeviceContextMutex->unlock();
			//l_DeviceContext->GSSetConstantBuffers(0, 1, &l_ConstantBufferGS);
			l_DeviceContextMutex->lock();
			l_DeviceContext->GSSetConstantBuffers(0, 4, GSBuffers);
			l_DeviceContextMutex->unlock();
		}
		else
		{
			l_DeviceContextMutex->lock();
			l_DeviceContext->GSSetShader(NULL, NULL, 0);
			l_DeviceContextMutex->unlock();
		}
		l_DeviceContextMutex->lock();
		l_DeviceContext->PSSetShader(l_EffectPixelShader->GetPixelShader(), NULL, 0);
		l_DeviceContextMutex->unlock();
		
		ID3D11Buffer *l_SceneConstantBufferPS=l_EffectPixelShader->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_LightConstantBufferPS=l_EffectPixelShader->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_AnimationConstantBufferPS=l_EffectPixelShader->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_MaterialParametersConstantBufferPS = l_EffectPixelShader->GetConstantBuffer(MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID);

		l_DeviceContextMutex->lock();
		l_DeviceContext->UpdateSubresource(l_MaterialParametersConstantBufferPS, 0, NULL, _Parameters, 0, 0);
		l_DeviceContextMutex->unlock();

		ID3D11Buffer* PSBuffers[4] = { l_SceneConstantBufferPS, l_LightConstantBufferPS, l_AnimationConstantBufferPS, l_MaterialParametersConstantBufferPS };
		l_DeviceContextMutex->lock();
		l_DeviceContext->PSSetConstantBuffers(0, 4, PSBuffers);
		l_DeviceContextMutex->unlock();

		l_DeviceContextMutex->lock();
		l_DeviceContext->Draw(l_VertexsToRender, 0);
		l_DeviceContextMutex->unlock();
		return true;
	}

	bool UpdateVertexs(void *Vtxs, unsigned int VtxsCount)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(mappedResource));

		ID3D11DeviceContext * l_DeviceContext = UABEngine.GetInstance()->GetRenderManager()->GetDeviceContext();
		std::mutex* l_DeviceContextMutex = &(UABEngine.GetMutexManager()->g_DeviceContextMutex);
		l_DeviceContextMutex->lock();
		HRESULT l_HR = l_DeviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		l_DeviceContextMutex->unlock();
		if (FAILED(l_HR))
			return false;

		memcpy(mappedResource.pData, Vtxs, sizeof(T) * VtxsCount);

		l_DeviceContextMutex->lock();
		l_DeviceContext->Unmap(m_VertexBuffer, 0);
		l_DeviceContextMutex->unlock();

		return true;
	}


	const Vect3f* GetVertexs()const { return &m_Vtxs[0]; }
	const unsigned int GetNVertexs(){ return m_VertexsCount; }

	const unsigned int GetSizeOfVertex(){ return sizeof(T); }
};

#define CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(ClassName, TopologyType) \
template<class T> \
class ClassName : public CTemplatedRenderableVertexs<T> \
{ \
public: \
ClassName(void *Vtxs, unsigned int VtxsCount, unsigned int PrimitiveCount, bool Dynamic = false) \
: CTemplatedRenderableVertexs(Vtxs, VtxsCount, TopologyType, PrimitiveCount, Dynamic)\
{ \
} \
};

CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CUABPointsListRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CUABLinesListRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CUABTrianglesListRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CUABTrianglesStripRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

#endif TEMPLATED_RENDERABLE_VERTEX_H