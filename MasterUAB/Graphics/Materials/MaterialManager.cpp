#include "Materials\MaterialManager.h"
#include "XML\XMLTreeNode.h"

CMaterialManager::CMaterialManager()
{

}
CMaterialManager::~CMaterialManager()
{

}
void CMaterialManager::Load(const std::string &Filename)
{
	m_Filename = Filename;

	Destroy();

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
					AddResource(l_Element.GetPszProperty("name"),l_Material);

				}
			}
		}
	}
}
void CMaterialManager::Reload()
{
	std::map<std::string, std::string> l_MaterialNames;
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["materials"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("material"))
				{
					l_MaterialNames[l_Element.GetPszProperty("name")]="defined";
					AddUpdateResource(l_Element.GetPszProperty("name"), new CMaterial(l_Element));
				}
			}
		}
	}
	//RemoveAllBut(l_MaterialNames);
}