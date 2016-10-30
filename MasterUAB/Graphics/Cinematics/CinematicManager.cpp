#include "CinematicManager.h"
#include "XML\tinyxml2.h"
#include "Engine\UABEngine.h"
#include "LevelManager\LevelManager.h"

CCinematicManager::CCinematicManager(CLevel* _Level) :CRenderableObject(_Level)
{

}

CCinematicManager::~CCinematicManager()
{
	Destroy();
}

void CCinematicManager::LoadXML(const std::string &FileName, CLevel* _Level)
{
	m_FileName = FileName;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(FileName.c_str());

	tinyxml2::XMLElement* l_Element;
	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("cinematics");
		if (l_Element != NULL)
		{
			l_Element = l_Element->FirstChildElement();
			while (l_Element != NULL)
			{
				CCinematic* l_Cinematic = new CCinematic(l_Element, _Level);
				AddResource(l_Cinematic->GetName(), l_Cinematic, m_Level);
				l_Element = l_Element->NextSiblingElement();
			}
		}
	}
}

void CCinematicManager::Reload()
{
	Destroy();
	LoadXML(m_FileName, UABEngine.GetLevelManager()->GetResource(m_Level));
}

void CCinematicManager::Update(float _ElapsedTime)
{
	for (TMapResource::iterator iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
	{
		iterator->second->Update(_ElapsedTime);
	}
}