#include "InstanceMesh.h"
#include "Engine\UABEngine.h"
#include "Effects\EffectManager.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "StaticMeshManager.h"

#include "LevelManager\LevelManager.h"

#include "StaticMesh.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "PhysXManager\PhysXManager.h"

#include <d3d11.h>
#include "Math\Matrix44.h"


CInstanceMesh::CInstanceMesh(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :CRenderableObject(TreeNode,_Level)
{
	m_StaticMesh = _Level->GetStaticMeshManager()->GetResource(TreeNode->GetPszProperty("core_name"));
	m_Frustum = UABEngine.GetRenderManager()->GetFrustum();
	m_Layer = TreeNode->GetPszProperty("layer");
	m_GeneratePhysx = TreeNode->GetBoolProperty("create_physics");
	tinyxml2::XMLElement* l_Element;
	if (m_GeneratePhysx)
	{
		std::string l_Name = GetName();
		//char l_ActorName[256] = "";
		//sprintf_s(l_ActorName, "%s_%s", _LevelId.c_str(), l_Name.c_str());

		std::string l_ActorName = l_Name;
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
			std::string l_auxDirectoty = TreeNode->GetPszProperty("physx_mesh_directory", "");
			if (l_auxDirectoty  == "")
			{
				l_auxDirectoty = m_StaticMesh->GetPhysxMeshesDirectory();
			}
			l_PhysXManager->CreateStaticTriangleMesh(l_ActorName, m_Level, m_StaticMesh, l_auxDirectoty, m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
		}else if (m_PxType == "sphere_shape")
		{
			l_PhysXManager->CreateStaticSphere(l_ActorName, m_StaticMesh->GetBoundingSphereRadius(), m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
		}
		else if (m_PxType == "plane_shape")
		{
			m_PxNormals = TreeNode->GetVect3fProperty("physics_normal", Vect3f(.0f, 1.f, .0f));
			m_PxOffset = TreeNode->GetFloatProperty("physics_offset", .0f);
			l_PhysXManager->CreateStaticPlane(l_ActorName, m_PxNormals, m_PxOffset, m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
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
				l_PhysXManager->CreateBoxTrigger(l_ActorName, l_BB, m_PxMaterial, l_Position, l_Rotation, m_PxGroup, l_OnTriggerEnterLuaFunction, l_OnTriggerStayLuaFunction, l_OnTriggerExitLuaFunction, l_ActivateActors, l_IsTriggerActive);
			else
				l_PhysXManager->CreateSphereTrigger(l_ActorName, m_StaticMesh->GetBoundingSphereRadius(), m_PxMaterial, l_Position, l_Rotation, m_PxGroup, l_OnTriggerEnterLuaFunction, l_OnTriggerStayLuaFunction, l_OnTriggerExitLuaFunction, l_ActivateActors, l_IsTriggerActive);
		}
		else if (m_PxType == "convex_mesh")
		{
			l_PhysXManager->CreateStaticConvexMesh(l_ActorName, m_Level, m_StaticMesh, m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
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
			l_PhysXManager->CreateStaticBox(l_ActorName, l_BB, m_PxMaterial, l_Position, l_Rotation, m_PxGroup);
		}
	}
}

CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName, CLevel* _Level) :CRenderableObject(_Level)
{
	SetName(Name);
	m_StaticMesh = _Level->GetStaticMeshManager()->GetResource(CoreName);
	m_Frustum = UABEngine.GetRenderManager()->GetFrustum();
}


CInstanceMesh::~CInstanceMesh(void)
{
	CPhysXManager* l_PhysXManager = UABEngine.GetPhysXManager();
	if (m_GeneratePhysx)
	{
		if (m_PxType == "convex_mesh" || m_PxType == "triangle_mesh")
		{
			size_t l_NunMeshes = m_StaticMesh->GetRenderableVertexs().size();
			for (size_t i = 0; i < l_NunMeshes; i++)
			{
				char l_ActorName[256] = "";
				sprintf_s(l_ActorName, "%s_%s_%u",m_Level.c_str() ,GetName().c_str(), i);
				l_PhysXManager->RemoveActor(l_ActorName);
			}
		}
		else
		{
			char l_ActorName[256] = "";
			sprintf_s(l_ActorName, "%s_%s", m_Level.c_str(), GetName().c_str());
			l_PhysXManager->RemoveActor(l_ActorName);
		}
	}
	CRenderableObject::~CRenderableObject();
} 

void CInstanceMesh::Render(CRenderManager *RM)
{
	CRenderableObject::Render(RM);
	if (GetVisible())
	{
		if (GetInsideFrustum() || !UABEngine.GetFrustumActive())
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
	if (m_IsCinematic)
	{
		m_ScaleMatrix.SetIdentity();
		m_ScaleMatrix.Scale(m_Scale.x, m_Scale.y, m_Scale.z);

		m_RotationMatrix.SetIdentity();
		m_RotationMatrix = m_Rotation.rotationMatrix();

		m_TranslationMatrix.SetIdentity();
		m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

		m_TransformMatrix = m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;

		m_AnimatedScaleMatrix.SetIdentity();
		m_AnimatedScaleMatrix.Scale(m_AnimatedScale.x, m_AnimatedScale.y, m_AnimatedScale.z);

		m_AnimatedRotationMatrix.SetIdentity();
		m_AnimatedRotationMatrix = m_AnimatedRotation.rotationMatrix();

		m_AnimatedTranslationMatrix.SetIdentity();
		m_AnimatedTranslationMatrix.SetPos(m_AnimatedPosition.x, m_AnimatedPosition.y, m_AnimatedPosition.z);

		m_TransformMatrix = m_AnimatedScaleMatrix * m_AnimatedRotationMatrix * m_AnimatedTranslationMatrix * m_TransformMatrix;
		
		Vect3f l_Position = m_TransformMatrix.GetPos();
		return m_Frustum->SphereVisible(l_Position + m_StaticMesh->GetBoundingSphereCenter(), m_StaticMesh->GetBoundingSphereRadius());
	}
	else
	{
		return m_Frustum->SphereVisible(m_Position + m_StaticMesh->GetBoundingSphereCenter(), m_StaticMesh->GetBoundingSphereRadius());
	}
}

void CInstanceMesh::ChangeLevel(const std::string _NewLevel)
{
	CPhysXManager* l_PhysXManager = UABEngine.GetPhysXManager();
	if (m_GeneratePhysx)
	{
		if (m_PxType == "convex_mesh" || m_PxType == "triangle_mesh")
		{
			size_t l_NunMeshes = m_StaticMesh->GetRenderableVertexs().size();
			for (size_t i = 0; i < l_NunMeshes; i++)
			{
				char l_ActorName[256] = "";
				char l_NewActorName[256] = "";
				sprintf_s(l_ActorName, "%s_%s_%u", m_Level.c_str(), GetName().c_str(), i);
				sprintf_s(l_ActorName, "%s_%s_%u", _NewLevel.c_str(), GetName().c_str(), i);
				l_PhysXManager->ChangeActorName(l_ActorName, l_NewActorName);
			}
		}
		else
		{
			char l_ActorName[256] = "";
			char l_NewActorName[256] = "";
			sprintf_s(l_ActorName, "%s_%s", m_Level.c_str(), GetName().c_str());
			sprintf_s(l_ActorName, "%s_%s", _NewLevel.c_str(), GetName().c_str());
			l_PhysXManager->ChangeActorName(l_ActorName, l_NewActorName);
		}
	}
	m_Level = _NewLevel;
}

std::string CInstanceMesh::GetCoreName()
{
	return m_StaticMesh->GetName();
}