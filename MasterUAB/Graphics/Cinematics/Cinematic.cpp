#include "Cinematic.h"
#include "Cinematics\CinematicObject.h"

CCinematic::CCinematic()
{

}

CCinematic::~CCinematic()
{
	if(!m_CinematicObjects.empty())
	{
		for(int i=0; i<m_CinematicObjects.size();i++)
		{
			delete m_CinematicObjects[i];
		}
	}

}

void CCinematic::LoadXML(const std::string &FileName)
{
	m_Playing=false;
	m_CurrentTime=0;
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["cinematic"];
		if (l_Input.Exists())
		{
			m_Name=l_Input.GetPszProperty("name");
			m_Duration=l_Input.GetFloatProperty("duration", 0);
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
	if(m_Playing)
	{
		m_CurrentTime+=_ElapsedTime;
		if(!IsFinished())
		{
			for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
			{
				m_CinematicObjects[i]->Update(_ElapsedTime);
			}
		}
		else
		{
			if(m_Cycle)
			{
				OnRestartCycle();
			}
			else
			{
				m_Playing=false;
			}
		}
	}
}

void CCinematic::Play(bool _cycle)
{
	m_Playing=true;
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->Play(_cycle);
	}
}

void CCinematic::Stop()
{
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->Stop();
	}
}

void CCinematic::Pause()
{
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->Pause();
	}
}

void CCinematic::OnRestartCycle()
{
	m_Playing=true;
	m_CurrentTime=0;
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->OnRestartCycle();
	}
}

