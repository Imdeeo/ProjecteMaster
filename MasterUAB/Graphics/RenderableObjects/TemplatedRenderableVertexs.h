#ifndef TEMPLATED_RENDERABLE_VERTEX_H
#define TEMPLATED_RENDERABLE_VERTEX_H

#include <d3d11.h>
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableVertexs.h"

template<class T>
class CTemplatedRenderableVertexs : public CRenderableVertexs
{
private:
	ID3D11Buffer *m_VertexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	unsigned int m_VertexsCount;
	unsigned int m_PrimitiveCount;
public:
	CTemplatedRenderableVertexs(void *Vtxs, unsigned int VtxsCount,	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, unsigned int PrimitiveCount)
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
		ID3D11Device *l_Device=UABEngine.GetRenderManager()->GetDevice();
		HRESULT l_HR=l_Device->CreateBuffer(&l_BufferDescription, &InitData,
		&m_VertexBuffer);
		if(FAILED(l_HR))
			return;
	}
	virtual ~CTemplatedRenderableVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
	}
	bool Render(CRenderManager *RenderManager, CEffectTechnique	*EffectTechnique, void *_Parameters)
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

		ID3D11Buffer *l_SceneConstantBufferVS=l_EffectVertexShader->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_LightConstantBufferVS=l_EffectVertexShader->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_AnimationConstantBufferVS=l_EffectVertexShader->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_MaterialParametersConstantBufferVS = l_EffectVertexShader->GetConstantBuffer(MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID);

		/*OJUCUIDAO*/
		/*CContextManager* l_ContextManager = UABEngine.GetRenderManager()->GetContextManager();
		l_DeviceContext->RSSetState(l_ContextManager->GetRasterizerState(CContextManager::RS_SOLID_BACK_CULL));
		l_DeviceContext->OMSetDepthStencilState(l_ContextManager->GetDepthStencilState(CContextManager::DSS_DEPTH_ON), 0);
		Vect4f v(1, 1, 1, 1);
		l_DeviceContext->OMSetBlendState(l_ContextManager->GetBlendState(CContextManager::BLEND_CLASSIC), &v.x, 0xffffffff);*/
				
		CEffectManager* l_EffectManagerInstance = UABEngine.GetEffectManager();

		l_DeviceContext->UpdateSubresource(l_MaterialParametersConstantBufferVS, 0, NULL, _Parameters, 0, 0);
		ID3D11Buffer* VSBuffers[4] = { l_SceneConstantBufferVS, l_LightConstantBufferVS, l_AnimationConstantBufferVS, l_MaterialParametersConstantBufferVS };
		l_DeviceContext->VSSetConstantBuffers(0, 4,VSBuffers);

		l_DeviceContext->PSSetShader(l_EffectPixelShader->GetPixelShader(), NULL, 0);
		
		ID3D11Buffer *l_SceneConstantBufferPS=l_EffectPixelShader->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_LightConstantBufferPS=l_EffectPixelShader->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_AnimationConstantBufferPS=l_EffectPixelShader->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_MaterialParametersConstantBufferPS = l_EffectVertexShader->GetConstantBuffer(MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID);

		l_DeviceContext->UpdateSubresource(l_MaterialParametersConstantBufferPS, 0, NULL, _Parameters, 0, 0);

		ID3D11Buffer* PSBuffers[4] = { l_SceneConstantBufferPS, l_LightConstantBufferPS, l_AnimationConstantBufferPS, l_MaterialParametersConstantBufferPS };
		l_DeviceContext->PSSetConstantBuffers(0, 4, PSBuffers);

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
: CTemplatedRenderableVertexs(Vtxs, VtxsCount, TopologyType, PrimitiveCount)\
{ \
} \
};

CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CUABLinesListRenderableVertexs,D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CUABTrianglesListRenderableVertexs,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CUABTrianglesStripRenderableVertexs,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

#endif TEMPLATED_RENDERABLE_VERTEX_H