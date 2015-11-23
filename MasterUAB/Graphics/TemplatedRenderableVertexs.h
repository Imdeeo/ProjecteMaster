#include <d3d11.h>
#include "RenderableVertexs.h"

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
		ID3D11Device *l_Device=UABEngine.GetRenderManager().GetDevice();
		ID3D11Device *l_Device;
		HRESULT l_HR=l_Device->CreateBuffer(&l_BufferDescription, &InitData,
		&m_VertexBuffer);
		if(FAILED(l_HR))
			return;
	}
	virtual ~CTemplatedRenderableVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
	}
	bool Render(CRenderManager *RenderManager, CEffectTechnique	*EffectTechnique, void *Parameters)
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
		l_DeviceContext->UpdateSubresource(l_ConstantBufferVS, 0, NULL,	Parameters, 0, 0 );
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
: CTemplatedRenderableVertexs(Vtxs, VtxsCount, TopologyType, PrimitiveCount)\
{ \
} \
};