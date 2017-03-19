#include "Materials\MaterialManager.h"
#include "XML\tinyxml2.h"
#include <stdio.h>

#include "Engine\UABEngine.h"
#include "LevelManager\LevelManager.h"

CMaterialManager::CMaterialManager()
{

}
CMaterialManager::~CMaterialManager()
{

}
void CMaterialManager::Load(const std::string &LevelMaterialsFilename, CLevel* _Level, const std::string &DefaultMaterialsFilename)
{
	m_LevelMaterialsFilename = LevelMaterialsFilename;
	m_DefaultMaterialsFilename = DefaultMaterialsFilename;

	m_LevelId = _Level->GetName();
	//Destroy();

	LoadMaterialsFromFile(LevelMaterialsFilename, _Level, false, nullptr);
	/*if (DefaultMaterialsFilename != "")
		LoadMaterialsFromFile(DefaultMaterialsFilename,_LevelId,false,nullptr);*/
}

void CMaterialManager::Reload()
{
	std::map<std::string, std::string> l_MaterialNames;
	/*if (m_DefaultMaterialsFilename != "") {
		LoadMaterialsFromFile(m_DefaultMaterialsFilename, m_LevelId , true, &l_MaterialNames);
	}*/
	LoadMaterialsFromFile(m_LevelMaterialsFilename, UABEngine.GetLevelManager()->GetResource(m_LevelId), true, &l_MaterialNames);
	//RemoveAllBut(l_MaterialNames);
}

void CMaterialManager::LoadMaterialsFromFile(const std::string &Filename, CLevel* _Level, bool Update, std::map<std::string, std::string> *UpdatedNames)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(Filename.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("materials");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux!=NULL)
			{
				if (l_ElementAux->Name() == std::string("material"))
				{
					CMaterial* l_Material = new CMaterial(l_ElementAux,_Level->GetName());
					if (Update)
					{
						AddUpdateResource(l_ElementAux->GetPszProperty("name"), l_Material);
					}
					else {
						AddResource(l_ElementAux->GetPszProperty("name"), l_Material, _Level->GetName());
					}
					if (UpdatedNames != nullptr)
					{
						(*UpdatedNames)[l_ElementAux->GetPszProperty("name")] = "defined";
					}
				}
				l_ElementAux = l_ElementAux->NextSiblingElement();
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