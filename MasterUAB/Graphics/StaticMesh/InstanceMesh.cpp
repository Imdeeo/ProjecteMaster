#include "StaticMesh\InstanceMesh.h"
#include "Engine\UABEngine.h"
#include "XML\XMLTreeNode.h"
#include "PhysXManager\PhysXManager.h"
#include <d3d11.h>
#include "Math\Matrix44.h"


CInstanceMesh::CInstanceMesh(const CXMLTreeNode &TreeNode):CRenderableObject(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	m_StaticMesh = UABEngine.GetStaticMeshManager()->GetResource(l_Element.GetPszProperty("core_name"));
	CPhysXManager* l_PhysXManager = UABEngine.GetPhysXManager();
	Vect3f l_BBMax = m_StaticMesh->GetBoundingBoxMax();
	Vect3f l_BBMin = m_StaticMesh->GetBoundingBoxMin();
	Vect3f l_BB = Vect3f(9, 4.5, 9);
	l_PhysXManager->CreateStaticBox(GetName(), l_BB, "FisicasAux", GetPosition(), qfIDENTITY, 1);
}

CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName) :CRenderableObject()
{
	SetName(Name);
	m_StaticMesh = UABEngine.GetStaticMeshManager()->GetResource(CoreName);
}


CInstanceMesh::~CInstanceMesh(void)
{
	CRenderableObject::~CRenderableObject();
}

void CInstanceMesh::Render(CRenderManager *RM)
{
	CRenderableObject::Render(RM);
	if(GetVisible())
	{
		/*OJUCUIDAO*/ //Exportem els vertex dels objectes en un espai real, per tant si els transformem amb pos rot i scale es lia
					  //Exportamos los vertices de los objetos en espacio real, asi que al aplicarles la transformada con pos rot y scale, se lia parda
		RM->GetContextManager()->SetWorldMatrix(GetTransform());
		/*Mat44f world;
		world.SetIdentity();
		RM->GetContextManager()->SetWorldMatrix(world);*/
		m_StaticMesh->Render(RM);
	}
}

void CInstanceMesh::RenderDebug(CRenderManager *RM)
{
	if (GetDebugRender())
	{
		CRenderableObject::RenderDebug(RM);
		RM->GetContextManager()->SetWorldMatrix(GetTransform());
		CEffectTechnique * l_ET = RM->GetDebugRender()->GetDebugTechnique();
		CEffectManager::SetSceneConstants(l_ET);
		RM->GetDebugRender()->GetAxis()->Render(RM, l_ET, CEffectManager::GetRawData());
	}
}