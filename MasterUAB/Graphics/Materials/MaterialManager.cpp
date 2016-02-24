#include "Materials\MaterialManager.h"
#include "XML\XMLTreeNode.h"

static void __stdcall ReloadMaterial(void* _app)
{
	CMaterial* l_material = (CMaterial*)_app;
}

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
					AddResource(l_Element.GetPszProperty("name"),new CMaterial(l_Element));
				}
			}
		}
	}
}
void CMaterialManager::Reload()
{
	std::map<std::string, std::string> m_MaterialNames;
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
					m_MaterialNames[l_Element.GetPszProperty("name")]="defined";
					AddUpdateResource(l_Element.GetPszProperty("name"), new CMaterial(l_Element));
				}
			}
		}
	}
	RemoveAllBut(m_MaterialNames);
}