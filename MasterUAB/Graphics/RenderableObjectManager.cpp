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

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position)
{
	CInstanceMesh instanceMesh(InstanceName,CoreMeshName);
	instanceMesh.SetPosition(Position);
	if (AddResource(InstanceName, &instanceMesh))
		return &instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)
{
	return nullptr;
}

bool CRenderableObjectsManager::AddResource(const std::string &Name, CRenderableObject *RenderableObject)
{
	return CTemplatedVectorMapManager::AddResource(Name, RenderableObject);
}

void CRenderableObjectsManager::Load(const std::string &FileName)
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
					AddMeshInstance(l_Element.GetPszProperty("core_name"), l_Element.GetPszProperty("name"), l_Element.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f), true));
				}
			}
		}
	}
}

//void CRenderableObjectsManager::CleanUp(); ---no hacer

//CRenderableObject * CRenderableObjectsManager::GetInstance(const std::string &Name) const; ---no hacer