#include "AnimatedModelsManager.h"
#include "XML\XMLTreeNode.h"

CAnimatedModelsManager::CAnimatedModelsManager(void)
{
	//CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS /*| LOADER_INVERT_V_COORD*/);
}

CAnimatedModelsManager::~CAnimatedModelsManager(void)
{
}


bool CAnimatedModelsManager::Load(const std::string &Filename)
{
	m_Filename = Filename;

	std::string l_AnimatedModelName;
	std::string l_AnimatedModelPath;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(Filename.c_str()))
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
					CAnimatedCoreModel* l_AnimatedCoreModel = new CAnimatedCoreModel();
					l_AnimatedCoreModel->Load(l_AnimatedModelPath);
					AddResource(l_AnimatedModelName,l_AnimatedCoreModel);
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
	Destroy();
	return Load(m_Filename);
}