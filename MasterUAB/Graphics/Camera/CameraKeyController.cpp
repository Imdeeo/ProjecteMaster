<<<<<<< HEAD
<<<<<<< HEAD
#include "CameraKeyController.h"
#include "CameraKey.h"
#include "Camera\Camera.h"
=======
#include "Camera\CameraKeyController.h"
#include "Camera\Camera.h"

#include "Engine\UABEngine.h"
>>>>>>> develop
=======
#include "CameraKeyController.h"
#include "CameraKey.h"
#include "Camera\Camera.h"
#include "Utils.h"
#include <sstream>
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7

CCameraKeyController::CCameraKeyController(CXMLTreeNode &XMLTreeNode)
{
<<<<<<< HEAD
<<<<<<< HEAD
	ResetTime();
	m_TotalTime = XMLTreeNode.GetFloatProperty("total_time", 0,true);
	std::string l_Filename;
	l_Filename = XMLTreeNode.GetPszProperty("filename");
	LoadXML(l_Filename);
	
	m_Reverse = XMLTreeNode.GetBoolProperty("reverse");
	m_ReverseDirection = 1;
	m_Cycle = !m_Reverse;//*XMLTreeNode.GetPszProperty("cycle");
=======
	m_CurrentTime = 0;
	m_TotalTime = 30.0f/(*XMLTreeNode.GetPszProperty("total_time"));
=======
	ResetTime();
	m_TotalTime = XMLTreeNode.GetFloatProperty("total_time", 0,true);
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
	std::string l_Filename;
	l_Filename = XMLTreeNode.GetPszProperty("filename");
	LoadXML(l_Filename);
<<<<<<< HEAD
>>>>>>> develop
=======

	m_Reverse = XMLTreeNode.GetBoolProperty("reverse");
	m_ReverseDirection = 1;
	m_Cycle = !m_Reverse;//*XMLTreeNode.GetPszProperty("cycle");
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
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
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
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
<<<<<<< HEAD
		else if (m_ReverseDirection == 1)
=======
		else if (IsReverse() && m_ReverseDirection == 1)
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
		{
			m_ReverseDirection = -1;
			ResetTime();
		}
<<<<<<< HEAD
=======
		else
		{
			// Para otro tipo de cámara animada.
		}
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
	}	

	if (m_CurrentKey == 0 && IsReverse() && m_ReverseDirection == -1)
	{
		m_ReverseDirection = 1;
		ResetTime();
<<<<<<< HEAD
	}
	m_NextKey = m_CurrentKey + m_ReverseDirection * 1;

=======
	for(size_t i = 0; i < m_Keys.size(); i++){
		if (m_CurrentTime >= m_Keys[i]->m_Time){ m_CurrentKey = i; }
	}
	m_NextKey = m_CurrentKey+1;
	if(m_NextKey >= m_Keys.size()){ m_NextKey = 0; }
>>>>>>> develop
=======
	}
	m_NextKey = m_CurrentKey + m_ReverseDirection * 1;

>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
}

void CCameraKeyController::Update(float ElapsedTime)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
	SetCurrentTime(m_CurrentTime + ElapsedTime);
	GetCurrentKey();
	float l_CurrentTime;
	float l_tI;
	float l_tF;
	Vect3f l_pI;
	Vect3f l_pF;
<<<<<<< HEAD

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
	
	
=======
	m_CurrentTime += ElapsedTime;
	/*
	Determinar el tiempo que ha pasado, blabla, para poder
	obtener los parámetros actuales de la cámara, después,
	hacer todos los sets igual que en SetCamera, pero usando
	m_Camera, que es una referencia a la cámara del juego.
	*/
}
=======
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7

	float l_fI;
	float l_fF;

<<<<<<< HEAD
	Camera->SetMatrixs();
>>>>>>> develop
=======
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
	
	
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
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