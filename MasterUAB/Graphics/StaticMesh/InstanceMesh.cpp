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
		l_BB = Vect3f(abs(l_BB.x), abs(l_BB.y), abs(l_BB.z));
		Vect3f l_Position = GetPosition();
		Quatf l_Rotation = Quatf(-m_Rotation.x, m_Rotation.y, -m_Rotation.z, -m_Rotation.w);

		CPhysXManager* l_PhysXManager = UABEngine.GetPhysXManager();
		if (l_PxType == "triangle_mesh")
		{
			std::vector<Vect3f> l_Vertexs;
			l_PhysXManager->CreateComplexStaticShape(l_Name, l_Vertexs, l_PxMaterial, l_Position, l_Rotation, l_PxGroup);
		}else if (l_PxType == "sphere_shape")
		{
			l_PhysXManager->CreateStaticSphere(l_Name, m_StaticMesh->GetBoundingSphereRadius(), l_PxMaterial, l_Position, l_Rotation, l_PxGroup);
		}
		else if (l_PxType == "plane_shape")
		{
			Vect3f l_Normal = TreeNode.GetVect3fProperty("physics_normal", Vect3f(.0f, 1.f, .0f), true);
			float l_Offset = TreeNode.GetFloatProperty("physics_offset", .0f, true);
			l_PhysXManager->CreateStaticPlane(l_Name, l_Normal, l_Offset, l_PxMaterial, l_Position, l_Rotation, l_PxGroup);
		}
		else if (l_PxType == "box_trigger" || l_PxType == "sphere_trigger")
		{
			std::string l_OnTriggerLuaFunction = TreeNode.GetPszProperty("on_trigger");
			std::vector<std::string> l_ActivateActors;
			CXMLTreeNode l_Node = TreeNode;
			for (int i = 0; i < l_Node.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Node(i);
				if (l_Element.GetName() == std::string("active_actor"))
				{
					l_ActivateActors.push_back(l_Element.GetPszProperty("actor_name"));
				}
			}
			if (l_PxType == "box_trigger")
				l_PhysXManager->CreateBoxTrigger(l_Name, l_BB, l_PxMaterial, l_Position, l_Rotation, l_PxGroup, l_OnTriggerLuaFunction, l_ActivateActors);
			else
				l_PhysXManager->CreateSphereTrigger(l_Name, l_BB, l_PxMaterial, l_Position, l_Rotation, l_PxGroup, l_OnTriggerLuaFunction, l_ActivateActors);
		}
		else if (l_PxType == "box_shape")
		{
			if (l_BB.x <= 0)
			{
				l_BB.x = 0.001;
			}
			if (l_BB.y <= 0)
			{
				l_BB.y = 0.001;
			}
			if (l_BB.z <= 0)
			{
				l_BB.z = 0.001;
			}
			l_PhysXManager->CreateStaticBox(GetName(), l_BB, l_PxMaterial, l_Position, l_Rotation, l_PxGroup);
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

#ifdef _DEBUG
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
#endif

CInstanceMesh & CInstanceMesh::operator=(CInstanceMesh & _InstanceMesh)
{

	*((CRenderableObject*)this) = (CRenderableObject)_InstanceMesh;
	m_Name = _InstanceMesh.m_Name;
	return *(this);
}