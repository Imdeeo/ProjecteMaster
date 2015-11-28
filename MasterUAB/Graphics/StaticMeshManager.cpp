#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"

CStaticMeshManager::CStaticMeshManager(void)
{
}


CStaticMeshManager::~CStaticMeshManager(void)
{
}

bool CStaticMeshManager::Load(const std::string &FileName)
{
	m_Filename = FileName;
	std::string l_MeshName;
	std::string l_MeshFileName;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_Filename.c_str()))
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
					l_StaticMesh->Load(l_MeshFileName);

					AddResource(l_MeshName, l_StaticMesh);
				}
			}
		}
	}
	return true;
}

bool CStaticMeshManager::Reload()
{
	this->Reload();
	return false;
}