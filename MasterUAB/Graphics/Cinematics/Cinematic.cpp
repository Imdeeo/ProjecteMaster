#include "Cinematic.h"
#include "Cinematics\CinematicObject.h"

void CCinematic::LoadXML(const std::string &FileName)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["cinematic"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("cinematic_object"))
				{
					AddCinematicObject(new CCinematicObject(l_Element));
				}
			}
		}
	}	
}

void CCinematic::AddCinematicObject(CCinematicObject *CinematicObject)
{
	m_CinematicObjects.push_back(CinematicObject);
}

void CCinematic::Update(float _ElapsedTime)
{
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->Update(_ElapsedTime);
	}
}

void CCinematic::Render(CRenderManager &RenderManager){}

