#include "ManchasManager.h"
#include "XML\XMLTreeNode.h"

CManchasManager::CManchasManager(void){}

CManchasManager::~CManchasManager(void)
{
	//Destroy();
}


void CManchasManager::Load(const std::string &Filename)
{
	CXMLTreeNode l_File;
	m_Filename = Filename;

	if (l_File.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Meshes = l_File["manchas"];
		for (int i = 0; i < l_Meshes.GetNumChildren(); ++i)
		{
			CManchasSystemType *l_ManchasSystemType = new CManchasSystemType(l_Meshes(i));
			if (!AddResource(l_ManchasSystemType->GetName(), l_ManchasSystemType))
			{
				CHECKED_DELETE(l_ManchasSystemType);
			}
		}
	}
}

void CManchasManager::Reload()
{
	Destroy();
	Load(m_Filename);
}

/*void CManchasManager::Save()
{
	FILE* l_File;
	if (!fopen_s(&l_File, m_Filename.c_str(), "w"))
	{
		fprintf_s(l_File, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
		fprintf_s(l_File, "<particle_systems>\n");

		typedef TMapResource::iterator it_type;
		for (it_type iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
		{
			iterator->second->Save(l_File);
		}

		fprintf_s(l_File, "</particle_systems>\n");
		fclose(l_File);
	}
}*/