#include "BilboardManager.h"
#include "XML\tinyxml2.h"
#include "Engine\UABEngine.h"
#include "LevelManager\LevelManager.h"

CBilboardManager::CBilboardManager(void){}

CBilboardManager::~CBilboardManager(void)
{
	Destroy();
}

void CBilboardManager::Load(const std::string &Filename, CLevel* _Level)
{
	m_Filename = Filename;
	m_LevelId = _Level->GetName();

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(Filename.c_str());

	tinyxml2::XMLElement* l_Element;

	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("bilboards_systems");
		if (l_Element!=NULL)
		{
			l_Element = l_Element->FirstChildElement();
			while (l_Element != NULL)
			{
				CBilboardSystemType *l_BilboardSystemType = new CBilboardSystemType(l_Element, _Level);
				if (!AddResource(l_BilboardSystemType->GetName(), l_BilboardSystemType, m_LevelId))
				{
					CHECKED_DELETE(l_BilboardSystemType);
				}
				l_Element = l_Element->NextSiblingElement();
			}
		}
	}
}

void CBilboardManager::Reload()
{
	Destroy();
	Load(m_Filename, UABEngine.GetLevelManager()->GetResource(m_LevelId));
}