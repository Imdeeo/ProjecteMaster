#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"

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
	std::string l_PhysxMeshDirectory;

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
					l_MeshName = l_Element.GetPszProperty("name","", false);
					l_MeshFileName = l_Element.GetPszProperty("filename", "", false);
					l_PhysxMeshDirectory = l_Element.GetPszProperty("physx_mesh_directory", "", false);

					CStaticMesh *l_StaticMesh = new CStaticMesh;
					l_StaticMesh->SetName(l_MeshName);
					l_StaticMesh->SetPhysxMeshesDirectory(l_PhysxMeshDirectory);
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

bool CStaticMeshManager::Reload()
{
	typedef TMapResource::iterator it_type;
	for(it_type iterator = m_Resources.begin();iterator != m_Resources.end();iterator++)
	{
		iterator->second->Reload();
	}
	return false;
}