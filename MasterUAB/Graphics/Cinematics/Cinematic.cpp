#include "Cinematic.h"
#include "Cinematics\CinematicObject.h"

CCinematic::CCinematic(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CNamed(TreeNode), CLevelInfo(_Level)
{
	m_Playing = false;
	m_CurrentTime = 0.0f;
	m_Name = TreeNode->GetPszProperty("name");
	m_Duration = TreeNode->GetFloatProperty("duration", 0);
	m_Cycle = TreeNode->GetBoolProperty("cycle", false);

	tinyxml2::XMLElement* l_Element = TreeNode->FirstChildElement();
	while (l_Element != NULL)
	{
		if (l_Element->Name() == std::string("cinematic_object"))
		{
			AddCinematicObject(new CCinematicObject(l_Element, _Level));
		}
		l_Element = l_Element->NextSiblingElement();
	}
}

CCinematic::~CCinematic()
{
	if(!m_CinematicObjects.empty())
	{
		for(size_t i=0; i<m_CinematicObjects.size();i++)
		{
			delete m_CinematicObjects[i];
		}
	}

}

void CCinematic::AddCinematicObject(CCinematicObject *CinematicObject)
{
	m_CinematicObjects.push_back(CinematicObject);
}

void CCinematic::Update(float _ElapsedTime)
{
	if (m_Playing)
	{
		m_CurrentTime += _ElapsedTime;
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
				Pause();
			}
		}
	}
}

void CCinematic::Play()
{
	m_Playing = true;
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->Play(m_Cycle);
	}
}

void CCinematic::Stop()
{
	m_Playing = false;
	m_CurrentTime = 0.0f;
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->Stop();
	}
}

void CCinematic::Pause()
{
	m_Playing = false;
	m_CurrentTime = 0.0f;
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->Pause();
	}
}

void CCinematic::OnRestartCycle()
{
	m_Playing = true;
	m_CurrentTime = 0.0f;
	for (size_t i = 0; i<m_CinematicObjects.size(); ++i)
	{
		m_CinematicObjects[i]->OnRestartCycle();
	}
}

bool CCinematic::IsFinished()
{
	return (m_CurrentTime >= m_Duration);
}