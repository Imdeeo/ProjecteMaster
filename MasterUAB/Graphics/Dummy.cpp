#include "TemplatedRenderableVertexs.h"
#include "TemplatedRenderableIndexedVertexs.h"

void things()
{
	void *h;
	CTemplatedRenderableVertexs<int> a(h,0,D3D11_PRIMITIVE_TOPOLOGY_LINELIST,0);
	CTemplatedRenderableIndexedVertexs<int>(h,0,h,0,D3D11_PRIMITIVE_TOPOLOGY_LINELIST,DXGI_FORMAT_R16_UINT);
}