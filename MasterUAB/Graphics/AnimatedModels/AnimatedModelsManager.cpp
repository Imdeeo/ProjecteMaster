#include "AnimatedModelsManager.h"
#include "XML\XMLTreeNode.h"

CAnimatedModelsManager::CAnimatedModelsManager(void)
{
}


CAnimatedModelsManager::~CAnimatedModelsManager(void)
{
}

bool CAnimatedModelsManager::Load(const std::string &FileName)
{
	m_Filename = FileName;
	std::string l_AnimatedModelName;
	std::string l_AnimatedModelPath;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["animated_models"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("animated_model"))
				{
					l_AnimatedModelName = l_Element.GetPszProperty("name");
					l_AnimatedModelPath = l_Element.GetPszProperty("path");
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

bool CAnimatedModelsManager::Reload()
{
	//hacer rama local
	return true;
}
