#include "InstanceMesh.h"
#include "UABEngine.h"
#include "XML\XMLTreeNode.h"

#include <d3d11.h>
#include "Math\Matrix44.h"


CInstanceMesh::CInstanceMesh(const CXMLTreeNode &TreeNode):CRenderableObject(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	SetName(l_Element.GetPszProperty("name"));
	m_StaticMesh = UABEngine.GetStaticMeshManager()->GetResource(l_Element.GetPszProperty("core_name"));
}

CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName)
{
	SetName(Name);
	m_StaticMesh = UABEngine.GetStaticMeshManager()->GetResource(CoreName);
}


CInstanceMesh::~CInstanceMesh(void)
{

}

void CInstanceMesh::Render(CRenderManager *RM)
{
	if(GetVisible())
	{
		/*OJUCUIDAO*/ //Exportem els vertex dels objectes en un espai real, per tant si els transformem amb pos rot i scale es lia
					  //Exportamos los vertices de los objetos en espacio real, asi que al aplicarles la transformada con pos rot y scale, se lia parda
		//RM->GetContextManager()->SetWorldMatrix(GetTransform());
		Mat44f world;
		world.SetIdentity();
		RM->GetContextManager()->SetWorldMatrix(world);
		m_StaticMesh->Render(RM);
	}
}