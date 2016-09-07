#include "Camera\CameraKeyController.h"
#include "Camera\Camera.h"
#include "Camera\CameraInfo.h"
#include "Engine\UABEngine.h"
#include "CameraKey.h"
#include "Utils.h"
#include <sstream>

CCameraKeyController::CCameraKeyController(tinyxml2::XMLElement* TreeNode) : CCameraController(TreeNode)
{
	ResetTime();
	m_PositionOffset = Vect3f(0.0f, 0.0f, 0.0f);
	m_TotalTime = TreeNode->GetFloatProperty("total_time", 0);
	std::string l_Filename;
	l_Filename = TreeNode->GetPszProperty("filename");
	LoadXML(l_Filename);

	m_Reverse = TreeNode->GetBoolProperty("reverse");
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

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(FileName.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("camera_key_controller");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux != NULL)
			{
				if (l_ElementAux->Name() == std::string("key"))
				{
					l_Time = std::stof(l_ElementAux->GetPszProperty("time"));

					CCameraInfo *l_CameraInfo = new CCameraInfo(l_ElementAux);
					CCameraKey *l_CameraKey = new CCameraKey(*l_CameraInfo, l_Time);

					m_Keys.push_back(l_CameraKey);
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
			// Para otro tipo de c�mara animada.
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
	m_NextKey = (size_t)((float)m_CurrentKey + m_ReverseDirection * 1);

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

	Vect3f l_uI;
	Vect3f l_uF;

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

		l_uI = m_Keys[m_CurrentKey]->m_CameraInfo.m_Up;
		l_uF = m_Keys[m_NextKey]->m_CameraInfo.m_Up;
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

		l_uI = m_Keys[m_NextKey]->m_CameraInfo.m_Up;
		l_uF = m_Keys[m_CurrentKey]->m_CameraInfo.m_Up;
	}

	m_Position = (((l_pF - l_pI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_pI;
	m_Fov = (((l_fF - l_fI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_fI;
	m_LookAt = (((l_lF - l_lI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_lI;
	m_Up = (((l_uF - l_uI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_uI;
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
	Camera->SetFOV(m_Fov);
	Camera->SetAspectRatio(16.0f / 9.0f);
	Camera->SetPosition(m_Position + m_PositionOffset);
	Camera->SetLookAt(m_LookAt + m_PositionOffset);
	Camera->SetUp(m_Up);
	Camera->SetMatrixs();
	Vect3f l_LA = m_LookAt + m_PositionOffset;
	UtilsLog("Cam LA: " + std::to_string(l_LA.x) + ", " + std::to_string(l_LA.y) + ", " + std::to_string(l_LA.z));
}

Vect3f CCameraKeyController::GetLastLookAt()
{
	return m_Keys[m_Keys.size()-1]->m_CameraInfo.m_LookAt;
}

void CCameraKeyController::SetFirstKey(Vect3f _Forward, Vect3f _Pos, Vect3f _Up, float _Fov)
{
	m_Fov = _Fov;
	m_Position = _Pos - m_PositionOffset;
	m_LookAt = _Forward + m_Position;
	m_Up = _Up;
	m_Keys[0]->m_CameraInfo.m_FOV = _Fov;
	m_Keys[0]->m_CameraInfo.m_Eye = _Pos - m_PositionOffset;
	m_Keys[0]->m_CameraInfo.m_LookAt = _Forward + m_Keys[0]->m_CameraInfo.m_Eye;
	m_Keys[0]->m_CameraInfo.m_Up = _Up;
}

CCameraInfo* CCameraKeyController::GetLastKey()
{
	return &m_Keys[m_Keys.size()-1]->m_CameraInfo;
}