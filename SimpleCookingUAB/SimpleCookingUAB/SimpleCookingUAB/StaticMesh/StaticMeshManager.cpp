#include "StaticMeshManager.h"
#include "PhyxManager\PhysXManager.h"
#include "Base\XML\XMLTreeNode.h"

#include <map>

CStaticMeshManager::CStaticMeshManager(void)
{
}


CStaticMeshManager::~CStaticMeshManager(void)
{
	Destroy();
}

bool CStaticMeshManager::Load(const std::string &FileName)
{
	m_Filename = FileName;
	std::string l_MeshName;
	std::string l_MeshFileName;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["static_meshes"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("static_mesh"))
				{
					l_MeshName = l_Element.GetPszProperty("name");
					l_MeshFileName = l_Element.GetPszProperty("filename");

					CStaticMesh *l_StaticMesh = new CStaticMesh;
					l_StaticMesh->SetName(l_MeshName);
					l_StaticMesh->Load(l_MeshFileName);

					AddResource(l_MeshName, l_StaticMesh);
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

void CStaticMeshManager::CreatePhysXMeshes(CPhysXManager* _PhysXManager)
{
	CStaticMeshManager::TMapResource l_MeshMap = GetResourcesMap();
	CStaticMeshManager::TMapResource::iterator it;
	for (it = l_MeshMap.begin(); it != l_MeshMap.end(); it++)
	{
		_PhysXManager->CreateConvexMesh(it->second->GetName(), it->second);
		_PhysXManager->CreateTriangleMesh(it->second->GetName(), it->second);
	}
}