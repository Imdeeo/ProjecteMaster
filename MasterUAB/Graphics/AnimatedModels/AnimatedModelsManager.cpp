#include "AnimatedModelsManager.h"
#include "XML\tinyxml2.h"
#include <cal3d\loader.h>

CAnimatedModelsManager::CAnimatedModelsManager(void)
{
	CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS); // | LOADER_INVERT_V_COORD
}

CAnimatedModelsManager::~CAnimatedModelsManager(void)
{
}

bool CAnimatedModelsManager::Load(const std::string &Filename)
{
	m_Filename = Filename;

	std::string l_AnimatedModelName;
	std::string l_AnimatedModelPath;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(Filename.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("animated_models");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux != NULL)
			{
				if (l_ElementAux->Name() == std::string("animated_model"))
				{
					l_AnimatedModelName = l_ElementAux->GetPszProperty("name");
					l_AnimatedModelPath = l_ElementAux->GetPszProperty("path");
					CAnimatedCoreModel* l_AnimatedCoreModel = new CAnimatedCoreModel();
					l_AnimatedCoreModel->Load(l_AnimatedModelPath);
					AddResource(l_AnimatedModelName,l_AnimatedCoreModel);
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

bool CAnimatedModelsManager::Reload()
{
	Destroy();
	return Load(m_Filename);
}