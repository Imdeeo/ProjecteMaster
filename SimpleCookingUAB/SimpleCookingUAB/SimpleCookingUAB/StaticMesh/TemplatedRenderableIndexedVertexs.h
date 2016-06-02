#ifndef TEMPLATED_RENDERABLE_INDEXED_VERTEX_H
#define TEMPLATED_RENDERABLE_INDEXED_VERTEX_H

#include <d3d11.h>
#include "RenderableVertexs.h"

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
		for (int i = 0; i < VtxsCount; i++)
		{
			m_Vtxs[i] = *(Vect3f*)(&data[i]);
		}

		if (m_IndexType == DXGI_FORMAT_R16_UINT)
		{
			unsigned short *index_aux = (unsigned short*)malloc(sizeof(unsigned short)*IndexsCount);
			unsigned short *index_data = (unsigned short *)Indices;
			for (int i = 0; i < IndexsCount; i++)
			{
				index_aux[i] = index_data[i];
			}
			m_Indexs = (void*)index_aux;
		}
		else
		{
			unsigned int *index_aux = (unsigned int*)malloc(sizeof(unsigned int)*IndexsCount);
			unsigned int *index_data = (unsigned int*)Indices;
			for (int i = 0; i < IndexsCount; i++)
			{
				index_aux[i] = index_data[i];
			}
			m_Indexs = (void*)index_aux;
		}

	}
	virtual ~CTemplatedRenderableIndexedVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
		CHECKED_RELEASE(m_IndexBuffer);
		CHECKED_DELETE(m_Indexs);
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