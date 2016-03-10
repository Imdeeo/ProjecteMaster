
#include "Camera\CameraKeyController.h"
#include "Camera\Camera.h"
#include "Engine\UABEngine.h"
#include "CameraKey.h"
#include "Utils.h"
#include <sstream>
#include "XML\XMLTreeNode.h"

CCameraKeyController::CCameraKeyController(CXMLTreeNode &XMLTreeNode)
{
	ResetTime();
	m_TotalTime = XMLTreeNode.GetFloatProperty("total_time", 0,true);
	std::string l_Filename;
	l_Filename = XMLTreeNode.GetPszProperty("filename");
	LoadXML(l_Filename);

	m_Reverse = XMLTreeNode.GetBoolProperty("reverse");
	m_ReverseDirection = 1;
	m_Cycle = !m_Reverse;//*XMLTreeNode.GetPszProperty("cycle");
}

CCameraKeyController::~CCameraKeyController()
{
	for (size_t i=0; i < m_Keys.size(); ++i)
	{
		CHECKED_DELETE(m_Keys[i]);
	}
	m_Keys.clear();
}

bool CCameraKeyController::LoadXML(const std::string &FileName)
{
	float l_Time;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["camera_key_controller"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("key"))
				{
					l_Time = std::stof(l_Element.GetPszProperty("time"));

					CCameraInfo *l_CameraInfo = new CCameraInfo(l_Element);
					CCameraKey *l_CameraKey = new CCameraKey(*l_CameraInfo, l_Time);

					m_Keys.push_back(l_CameraKey);
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

void CCameraKeyController::GetCurrentKey()
{
	if (IsCycle() || m_ReverseDirection == 1)
	{
		for (size_t i = 0; i < m_Keys.size(); i++){
			if (m_CurrentTime >= m_Keys[i]->m_Time){ m_CurrentKey = i; }
		}
	}
	else
	{
		float l_CurrentTime = m_TotalTime - m_CurrentTime;
		for (int i = m_Keys.size()-1; i >= 0; i--)
		{
			if (l_CurrentTime <= m_Keys[i]->m_Time){ m_CurrentKey = i; }
		}
	}
	
	if (m_CurrentKey == (m_Keys.size()-1))
	{ 
		if (IsCycle())
		{
			m_CurrentKey = 0;
			ResetTime();
		}
		else if (IsReverse() && m_ReverseDirection == 1)
		{
			m_ReverseDirection = -1;
			ResetTime();
		}
		else
		{
			// Para otro tipo de cámara animada.
		}
	}	

	if (m_CurrentKey == 0 && IsReverse() && m_ReverseDirection == -1)
	{
		m_ReverseDirection = 1;
		ResetTime();
		for(size_t i = 0; i < m_Keys.size(); i++){
			if (m_CurrentTime >= m_Keys[i]->m_Time){ m_CurrentKey = i; }
		}
		m_NextKey = m_CurrentKey+1;
		if(m_NextKey >= m_Keys.size()){ m_NextKey = 0; }
	}
	m_NextKey = m_CurrentKey + m_ReverseDirection * 1;

}

void CCameraKeyController::Update(float ElapsedTime)
{
	SetCurrentTime(m_CurrentTime + ElapsedTime);
	GetCurrentKey();
	float l_CurrentTime;
	float l_tI;
	float l_tF;
	Vect3f l_pI;
	Vect3f l_pF;


	float l_fI;
	float l_fF;

	Vect3f l_lI;
	Vect3f l_lF;

	if (IsCycle() || m_ReverseDirection == 1)
	{
		l_CurrentTime = m_CurrentTime;

		l_tI = m_Keys[m_CurrentKey]->m_Time;
		l_tF = m_Keys[m_NextKey]->m_Time;

		l_pI = m_Keys[m_CurrentKey]->m_CameraInfo.m_Eye;
		l_pF = m_Keys[m_NextKey]->m_CameraInfo.m_Eye;

		l_fI = m_Keys[m_CurrentKey]->m_CameraInfo.m_FOV;
		l_fF = m_Keys[m_NextKey]->m_CameraInfo.m_FOV;

		l_lI = m_Keys[m_CurrentKey]->m_CameraInfo.m_LookAt;
		l_lF = m_Keys[m_NextKey]->m_CameraInfo.m_LookAt;


	}
	else
	{
		l_CurrentTime = m_TotalTime - m_CurrentTime;

		l_tI = m_Keys[m_NextKey]->m_Time;
		l_tF = m_Keys[m_CurrentKey]->m_Time;

		l_pI = m_Keys[m_NextKey]->m_CameraInfo.m_Eye;
		l_pF = m_Keys[m_CurrentKey]->m_CameraInfo.m_Eye;

		l_fI = m_Keys[m_NextKey]->m_CameraInfo.m_FOV;
		l_fF = m_Keys[m_CurrentKey]->m_CameraInfo.m_FOV;

		l_lI = m_Keys[m_NextKey]->m_CameraInfo.m_LookAt;
		l_lF = m_Keys[m_CurrentKey]->m_CameraInfo.m_LookAt;
	}

	m_Position = (((l_pF - l_pI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_pI;
	m_FOV = (((l_fF - l_fI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_fI;
	m_LookAt = (((l_lF - l_lI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_lI;
	
}

void CCameraKeyController::SetCurrentTime(float CurrentTime)
{
	m_CurrentTime = CurrentTime;
}

void CCameraKeyController::ResetTime()
{
	m_CurrentTime = 0;
}

float CCameraKeyController::GetTotalTime()
{
	return m_TotalTime;
}

bool CCameraKeyController::IsCycle() const
{
	return m_Cycle;
}

void CCameraKeyController::SetCycle(bool Cycle)
{
	m_Cycle = Cycle;
}

bool CCameraKeyController::IsReverse() const
{
	return m_Reverse;
}

void CCameraKeyController::SetReverse(bool Reverse)
{
	m_Reverse = Reverse;
}

void CCameraKeyController::SetCamera(CCamera *Camera) const
{
	Camera->SetFOV(m_FOV);
	Camera->SetAspectRatio(16.0f / 9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_LookAt);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}


Vect3f CCameraKeyController::GetDirection() const
{
	Vect3f l_Direction(cos(m_Yaw)*cos(m_Pitch), sin(m_Pitch), sin(m_Yaw)*cos(m_Pitch));
	return l_Direction;
}