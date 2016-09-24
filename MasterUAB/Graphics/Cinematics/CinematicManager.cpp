#include "CinematicManager.h"
#include "XML\tinyxml2.h"

CCinematicManager::CCinematicManager()
{

}

CCinematicManager::~CCinematicManager()
{
	Destroy();
}

void CCinematicManager::LoadXML(const std::string &FileName, const std::string &_LevelId)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(FileName.c_str());

	tinyxml2::XMLElement* l_Element;

	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("cinematics")->FirstChildElement();
		while (l_Element != NULL)
		{
			CCinematic* l_Cinematic = new CCinematic(l_Element);
			AddResource(l_Cinematic->GetName(), l_Cinematic,_LevelId);
			l_Element = l_Element->NextSiblingElement();
		}
	}
}


void CCinematicManager::Update(float _ElapsedTime)
{
	for (TMapResource::iterator iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
	{
		iterator->second->Update(_ElapsedTime);
	}
}