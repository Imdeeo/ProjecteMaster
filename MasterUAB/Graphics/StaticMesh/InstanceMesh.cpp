#include "InstanceMesh.h"
#include "Engine\UABEngine.h"
#include "Effects\EffectManager.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "StaticMeshManager.h"

#include "StaticMesh.h"
#include "RenderableObjects\RenderableVertexs.h"

#include "XML\XMLTreeNode.h"
#include "PhysXManager\PhysXManager.h"

#include <d3d11.h>
#include "Math\Matrix44.h"
#include "Utils.h"


CInstanceMesh::CInstanceMesh(const CXMLTreeNode &TreeNode):CRenderableObject(TreeNode)
{
	m_StaticMesh = UABEngine.GetStaticMeshManager()->GetResource(TreeNode.GetPszProperty("core_name"));

	bool l_GeneratePhysx = TreeNode.GetBoolProperty("create_physics");
	if (l_GeneratePhysx)
	{
		std::string l_Name = GetName();
		std::string l_PxType = TreeNode.GetPszProperty("physics_type");
		std::string l_PxMaterial = TreeNode.GetPszProperty("physics_material");
		int l_PxGroup = TreeNode.GetIntProperty("physics_group");
		Vect3f l_BB = m_StaticMesh->GetBoundingBoxMax() - m_StaticMesh->GetBoundingBoxMin();
		l_BB.x = abs(l_BB.x);
		l_BB.y = abs(l_BB.y);
		l_BB.z = abs(l_BB.z);

		CPhysXManager* l_PhysXManager = UABEngine.GetPhysXManager();
		if (l_PxType == "triangle_mesh")
		{
			std::vector<Vect3f> l_Vertexs;
			l_PhysXManager->CreateComplexStaticShape(l_Name, l_Vertexs, l_PxMaterial, GetPosition(), m_Rotation, l_PxGroup);
		}else if (l_PxType == "sphere_shape")
		{
			l_PhysXManager->CreateStaticSphere(l_Name, m_StaticMesh->GetBoundingSphereRadius(), l_PxMaterial, GetPosition(), m_Rotation, l_PxGroup);
		}
		else if (l_PxType == "plane_shape")
		{
			Vect3f l_Normal = Vect3f(.0f, 1.0f, .0f);
			float l_Distance = sqrt(l_BB.x*l_BB.x + l_BB.y*l_BB.y + l_BB.z*l_BB.z);

			l_PhysXManager->CreateStaticPlane(l_Name, l_Normal, l_Distance, l_PxMaterial, GetPosition(), m_Rotation, l_PxGroup);
		}
		else
		{
			l_PhysXManager->CreateStaticBox(GetName(), l_BB, l_PxMaterial, GetPosition(), m_Rotation, l_PxGroup);
		}
	}
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