#include "InstanceMesh.h"
#include "Engine\UABEngine.h"
#include "Effects\EffectManager.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "StaticMeshManager.h"

#include "StaticMesh.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "PhysXManager\PhysXManager.h"

#include <d3d11.h>
#include "Math\Matrix44.h"


CInstanceMesh::CInstanceMesh(tinyxml2::XMLElement* TreeNode) :CRenderableObject(TreeNode)
{
	m_StaticMesh = UABEngine.GetStaticMeshManager()->GetResource(TreeNode->GetPszProperty("core_name"));
	m_Frustum = UABEngine.GetRenderManager()->GetFrustum();
	m_Layer = TreeNode->GetPszProperty("layer");
	m_GeneratePhysx = TreeNode->GetBoolProperty("create_physics");
	tinyxml2::XMLElement* l_Element;
	if (m_GeneratePhysx)
	{
		std::string l_Name = GetName();
		m_PxType = TreeNode->GetPszProperty("physics_type");
		m_PxMaterial = TreeNode->GetPszProperty("physics_material");
		m_PxGroup = TreeNode->GetPszProperty("physics_group");
		m_PxOffset = 0.0f;
		m_PxNormals = Vect3f(0.0f, 0.0f, 0.0f);

		Vect3f l_BB = m_StaticMesh->GetBoundingBoxMax() - m_StaticMesh->GetBoundingBoxMin();
		l_BB = Vect3f(abs(l_BB.x), abs(l_BB.y), abs(l_BB.z));
		Vect3f l_Position = GetPosition();
		Quatf l_Rotation = Quatf(-m_Rotation.x, m_Rotation.y, -m_Rotation.z, -m_Rotation.w);

		CPhysXManager* l_PhysXManager = UABEngine.GetPhysXManager();
		if (m_PxType == "triangle_mesh")
		{
			bool l_FlipNormals = TreeNode->GetBoolProperty("physics_flip_normals");
			l_PhysXManager->CreateStaticTriangleMesh(GetName(), m_StaticMesh, m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
		}else if (m_PxType == "sphere_shape")
		{
			l_PhysXManager->CreateStaticSphere(l_Name, m_StaticMesh->GetBoundingSphereRadius(), m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
		}
		else if (m_PxType == "plane_shape")
		{
			m_PxNormals = TreeNode->GetVect3fProperty("physics_normal", Vect3f(.0f, 1.f, .0f));
			m_PxOffset = TreeNode->GetFloatProperty("physics_offset", .0f);
			l_PhysXManager->CreateStaticPlane(l_Name, m_PxNormals, m_PxOffset, m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
		}
		else if (m_PxType == "box_trigger" || m_PxType == "sphere_trigger")
		{
			std::string l_OnTriggerEnterLuaFunction = TreeNode->GetPszProperty("on_trigger_enter","");
			std::string l_OnTriggerStayLuaFunction = TreeNode->GetPszProperty("on_trigger_stay","");
			std::string l_OnTriggerExitLuaFunction = TreeNode->GetPszProperty("on_trigger_exit","");

			bool l_IsTriggerActive = TreeNode->GetBoolProperty("is_active",true);

			std::vector<std::string> l_ActivateActors;
			l_Element = TreeNode->FirstChildElement();
			while (l_Element != NULL)
			{
				if (l_Element->Name() == std::string("active_actor"))
				{
					l_ActivateActors.push_back(l_Element->GetPszProperty("actor_name"));
				}
				else if (l_Element->Name() == std::string("interactuable_object"))
				{
					m_InteractuableObject = l_Element->GetPszProperty("actor_name");
				}
				l_Element = l_Element->NextSiblingElement();
			}
			if (m_PxType == "box_trigger")
				l_PhysXManager->CreateBoxTrigger(l_Name, l_BB, m_PxMaterial, l_Position, l_Rotation, m_PxGroup, l_OnTriggerEnterLuaFunction,l_OnTriggerStayLuaFunction,l_OnTriggerExitLuaFunction, l_ActivateActors,l_IsTriggerActive);
			else
				l_PhysXManager->CreateSphereTrigger(l_Name, m_StaticMesh->GetBoundingSphereRadius(), m_PxMaterial, l_Position, l_Rotation, m_PxGroup, l_OnTriggerEnterLuaFunction, l_OnTriggerStayLuaFunction, l_OnTriggerExitLuaFunction, l_ActivateActors, l_IsTriggerActive);
		}
		else if (m_PxType == "convex_mesh")
		{
			l_PhysXManager->CreateStaticConvexMesh(GetName(), m_StaticMesh, m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
		}
		else if (m_PxType == "box_shape")
		{
			if (l_BB.x <= 0)
			{
				l_BB.x = 0.001f;
			}
			if (l_BB.y <= 0)
			{
				l_BB.y = 0.001f;
			}
			if (l_BB.z <= 0)
			{
				l_BB.z = 0.001f;
			}
			l_PhysXManager->CreateStaticBox(GetName(), l_BB, m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
		}
	}
}

CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName) :CRenderableObject()
{
	SetName(Name);
	m_StaticMesh = UABEngine.GetStaticMeshManager()->GetResource(CoreName);
	m_Frustum = UABEngine.GetRenderManager()->GetFrustum();
}


CInstanceMesh::~CInstanceMesh(void)
{
	CRenderableObject::~CRenderableObject();
} 

void CInstanceMesh::Render(CRenderManager *RM)
{
	CRenderableObject::Render(RM);
	if(GetVisible() && (GetInsideFrustum() || !UABEngine.GetFrustumActive()))
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

void CInstanceMesh::Save(FILE* _File, std::string _layer)
{
	if (m_GeneratePhysx)
	{
		fprintf_s(_File, "\t<instance_mesh name=\"%s\" layer=\"%s\" core_name=\"%s\" position=\"%f %f %f\" rotation=\"%f %f %f %f\" "
			"create_physics=\"true\" physics_type=\"%s\" physics_offset=\"%f\" physics_normal=\"%f %f %f\" physics_material=\"%s\" "
			"physics_group=\"%s\" visible=\"%s\"/>\n",
			m_Name.c_str(), _layer.c_str(), m_StaticMesh->GetName().c_str(), m_Position.x, m_Position.y, m_Position.z,
			m_Rotation.x, m_Rotation.y, m_Rotation.z, m_Rotation.w, m_PxType.c_str(), m_PxOffset,
			m_PxNormals.x, m_PxNormals.y, m_PxNormals.z, m_PxMaterial.c_str(), m_PxGroup.c_str(), m_Visible ? "true" : "false");
	}
	else
	{
		fprintf_s(_File, "\t<instance_mesh name=\"%s\" layer=\"%s\" core_name=\"%s\" position=\"%f %f %f\" "
			"rotation=\"%f %f %f %f\" create_physics=\"false\" visible=\"%s\"/>\n",
			m_Name.c_str(), _layer.c_str(), m_StaticMesh->GetName().c_str(), m_Position.x, m_Position.y, m_Position.z,
			m_Rotation.x, m_Rotation.y, m_Rotation.z, m_Rotation.w, m_Visible ? "true" : "false");		
	}
}

bool CInstanceMesh::GetInsideFrustum()
{
	if (m_Layer == "skybox")
		return true;
	//Vect3f l_TransformedPosition = C3DElement::GetTransform() * m_Position; //Se carga las posiciones de las instancias.
	return m_Frustum->SphereVisible(m_Position, m_StaticMesh->GetBoundingSphereRadius());
}