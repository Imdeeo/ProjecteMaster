#include "Materials\MaterialManager.h"
#include "XML\XMLTreeNode.h"
#include <stdio.h>

CMaterialManager::CMaterialManager()
{

}
CMaterialManager::~CMaterialManager()
{

}
void CMaterialManager::Load(const std::string &LevelMaterialsFilename, const std::string &DefaultMaterialsFilename)
{
	m_LevelMaterialsFilename = LevelMaterialsFilename;
	m_DefaultMaterialsFilename = DefaultMaterialsFilename;

	Destroy();


	if (DefaultMaterialsFilename != "")
	{
		LoadMaterialsFromFile(DefaultMaterialsFilename);
	}
	LoadMaterialsFromFile(LevelMaterialsFilename,true);
}

void CMaterialManager::Reload()
{
	std::map<std::string, std::string> l_MaterialNames;
	CXMLTreeNode l_XML;
	if (m_DefaultMaterialsFilename != "") {
		LoadMaterialsFromFile(m_DefaultMaterialsFilename, true, &l_MaterialNames);
	}
	LoadMaterialsFromFile(m_LevelMaterialsFilename, true, &l_MaterialNames);
	//RemoveAllBut(l_MaterialNames);
}

void CMaterialManager::LoadMaterialsFromFile(const std::string &Filename, bool Update, std::map<std::string, std::string> *UpdatedNames)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["materials"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("material"))
				{
					CMaterial* l_Material = new CMaterial(l_Element);
					if (Update)
					{
						AddUpdateResource(l_Element.GetPszProperty("name"), l_Material);
					}
					else {
						AddResource(l_Element.GetPszProperty("name"), l_Material);
					}
					if (UpdatedNames != nullptr)
					{
						(*UpdatedNames)[l_Element.GetPszProperty("name")] = "defined";
					}
				}
			}
		}
	}
}

void CMaterialManager::Save()
{
	FILE* l_File;
	if (!fopen_s(&l_File, m_LevelMaterialsFilename.c_str(),"w"))
	{
		fprintf_s(l_File, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
		fprintf_s(l_File, "<materials>\n");

		typedef TMapResource::iterator it_type;
		for (it_type iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
		{
			iterator->second->Save(l_File);
		}

		fprintf_s(l_File, "</materials>\n");
		fclose(l_File);
	}
}