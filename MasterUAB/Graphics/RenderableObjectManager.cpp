#include "RenderableObjectManager.h"
#include "InstanceMesh.h"

CRenderableObjectsManager::CRenderableObjectsManager() {}
CRenderableObjectsManager::~CRenderableObjectsManager(){}

void CRenderableObjectsManager::Update(float ElapsedTime)
{
	for (int i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}
void CRenderableObjectsManager::Render(CRenderManager *RM)
{
	for (int i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(RM);
	}
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position,const float _Yaw, const float _Pitch, const float _Roll,const float _Scale, const bool _Visible)
{
	CInstanceMesh instanceMesh(InstanceName,CoreMeshName);
	instanceMesh.SetPosition(Position);
	instanceMesh.SetYaw(_Yaw);
	instanceMesh.SetPitch(_Pitch);
	instanceMesh.SetRoll(_Roll);
	instanceMesh.SetScale(_Scale);
	instanceMesh.SetVisible(_Visible);
	if (AddResource(InstanceName, &instanceMesh))
		return &instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)
{
	return nullptr;
}

bool CRenderableObjectsManager::Load(const std::string &FileName)
{
	Destroy();

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["renderable_objects"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("renderable_object"))
				{
					AddMeshInstance(l_Element.GetPszProperty("core_name"),
						l_Element.GetPszProperty("name"),
						l_Element.GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f), true),
						l_Element.GetFloatProperty("yaw",0.f,true),
						l_Element.GetFloatProperty("pitch",0.f,true),
						l_Element.GetFloatProperty("roll",0.f,true),
						l_Element.GetFloatProperty("scale",1.f,true),
						l_Element.GetBoolProperty("visible",false,true));
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

//void CRenderableObjectsManager::CleanUp(); ---no hacer

//CRenderableObject * CRenderableObjectsManager::GetInstance(const std::string &Name) const; ---no hacer