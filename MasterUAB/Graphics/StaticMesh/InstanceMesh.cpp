#include "StaticMesh\InstanceMesh.h"
#include "Engine\UABEngine.h"
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
		//std::string l_PxType = TreeNode.GetPszProperty("physics_type");
		//std::string l_PxMaterial = TreeNode.GetPszProperty("physics_material");
		//int l_PxGroup = TreeNode.GetIntProperty("physics_group");
		//Vect3f l_BB = m_StaticMesh->GetBoundingBoxMax() - m_StaticMesh->GetBoundingBoxMin();
		Vect3f l_BBMin = m_StaticMesh->GetBoundingBoxMax();
		Vect3f l_BBMax = m_StaticMesh->GetBoundingBoxMin();
		Quatf l_Q = QuatFromYawPitchRoll(m_Yaw, m_Pitch, m_Roll);

		CPhysXManager* l_PhysXManager = UABEngine.GetPhysXManager();
		/*if (l_PxType == "triangle_mesh")
		{
			std::vector<Vect3f> l_Vertexs;
			l_PhysXManager->CreateComplexStaticShape(l_Name, l_Vertexs, l_PxMaterial, l_Pos, qfIDENTITY, l_PxGroup);
		}else if (l_PxType == "sphere_shape")
		{
			l_PhysXManager->CreateStaticSphere(l_Name, m_StaticMesh->GetBoundingSphereRadius(), l_PxMaterial, l_Pos, qfIDENTITY, l_PxGroup);
		}
		else if (l_PxType == "plane_shape")
		{
			Vect3f l_Normal = Vect3f(.0f, 1.0f, .0f);
			float l_Distance = 100.0f;

			l_PhysXManager->CreateStaticPlane(l_Name, l_Normal, l_Distance, l_PxMaterial, l_Pos, qfIDENTITY, l_PxGroup);
		}
		else
		{
			//l_PhysXManager->CreateDinamicBox(GetName(), l_BB, l_PxMaterial, GetPosition(), qfIDENTITY, 1.0f, l_PxGroup);
			l_PhysXManager->CreateStaticBox(GetName(), Vect3f(20.f,5.f,20.f), l_PxMaterial, GetPosition(), qfIDENTITY, l_PxGroup);
		}*/
		Vect3f aux = l_BBMax - l_BBMin;
		aux.x = abs(aux.x);
		aux.y = abs(aux.y);
		aux.z = abs(aux.z);
		Vect3f l_Pos = (l_BBMax + l_BBMin) / 2;
		l_PhysXManager->CreateRigidStatic(l_Name, aux, GetPosition(), l_Q, "FisicasAux");
	}
	else
	{
		float aux = 3;
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