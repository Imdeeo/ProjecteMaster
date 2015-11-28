#include "StaticMesh.h"
#include "RenderManager.h"

CStaticMesh::CStaticMesh(void):CNamed("")
{
}

CStaticMesh::~CStaticMesh(void)
{
}

bool CStaticMesh::Load(const std::string &FileName)
{
	m_Name = FileName;



	return true;
}

bool CStaticMesh::Reload()
{
	if(m_Name!="")
	{
		Destroy();
		Load(m_Name);
		return true;
	}
	else
	{
		return false;
	}
}

void CStaticMesh::Render(CRenderManager *RM) const
{

}

bool CStaticMesh::Destroy()
{
	m_NumVertexs = 0;
	m_NumFaces = 0;
	CHECKED_RELEASE(m_RVs);
	m_RVs.clear();
	CHECKED_RELEASE(m_Materials);
	m_Materials.clear();
}