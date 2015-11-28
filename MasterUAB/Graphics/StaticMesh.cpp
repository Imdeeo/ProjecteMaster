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
	Load(m_Name);
	return true;
}

void CStaticMesh::Render(CRenderManager *RM) const
{
	
}