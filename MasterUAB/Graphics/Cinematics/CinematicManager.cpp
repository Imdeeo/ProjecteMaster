#include "CinematicManager.h"
#include "XML\XMLTreeNode.h"

CCinematicManager::CCinematicManager()
{

}

CCinematicManager::~CCinematicManager()
{
	Destroy();
}

void CCinematicManager::LoadXML(const std::string &FileName)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["cinematics"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				CCinematic* l_Cinematic = new CCinematic(l_Element);
				AddResource(l_Cinematic->GetName(), l_Cinematic);
			}
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