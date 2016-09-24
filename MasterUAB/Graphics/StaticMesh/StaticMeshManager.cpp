#include "StaticMeshManager.h"
#include "XML\tinyxml2.h"

CStaticMeshManager::CStaticMeshManager(void)
{
}


CStaticMeshManager::~CStaticMeshManager(void)
{
	Destroy();
}

bool CStaticMeshManager::Load(const std::string &FileName, const std::string &_LevelId)
{
	m_Filename = FileName;
	std::string l_MeshName;
	std::string l_MeshFileName;
	std::string l_PhysxMeshDirectory;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(FileName.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("static_meshes");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux!=NULL)
			{
				if (l_ElementAux->Name() == std::string("static_mesh"))
				{
					l_MeshName = l_ElementAux->GetPszProperty("name", "");
					l_MeshFileName = l_ElementAux->GetPszProperty("filename", "");
					l_PhysxMeshDirectory = l_ElementAux->GetPszProperty("physx_mesh_directory", "");

					CStaticMesh *l_StaticMesh = new CStaticMesh;
					l_StaticMesh->SetName(l_MeshName);
					l_StaticMesh->SetPhysxMeshesDirectory(l_PhysxMeshDirectory);
					l_StaticMesh->Load(l_MeshFileName);

					AddResource(l_MeshName, l_StaticMesh,_LevelId);
				}
				l_ElementAux = l_ElementAux->NextSiblingElement();
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