#include "Camera\CameraKeyController.h"
#include "Camera\Camera.h"
#include "Camera\CameraInfo.h"
#include "Engine\UABEngine.h"
#include "CameraKey.h"
#include "Utils.h"
#include <sstream>
#include "CameraControllerManager.h"
#include "InputManager\InputManager.h"
#include "Math\Quatn.h"

CCameraKeyController::CCameraKeyController(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CCameraController(TreeNode,_LevelId)
{
	ResetTime();
	m_PositionOffsetKey = Vect3f(0.0f, 0.0f, 0.0f);
	m_PositionOffset = Vect3f(0.0f, 0.0f, 0.0f);
	m_RotationOffset = Mat33f(1, 0, 0, 0, 1, 0, 0, 0, 1);
	m_TotalTime = TreeNode->GetFloatProperty("total_time", 0);
	std::string l_Filename;
	l_Filename = TreeNode->GetPszProperty("filename");
	LoadXML(l_Filename);

	m_Reverse = TreeNode->GetBoolProperty("reverse");
	m_ReverseDirection = 1;
	m_Cycle = /*!m_Reverse;//*/TreeNode->GetBoolProperty("cycle");
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
			if (m_CurrentTime >= m_Keys[i]->GetTime()){ m_CurrentKey = i; }
		}
	}
	else
	{
		float l_CurrentTime = m_TotalTime - m_CurrentTime;
		for (int i = m_Keys.size()-1; i >= 0; i--)
		{
			if (l_CurrentTime <= m_Keys[i]->GetTime()){ m_CurrentKey = i; }
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
			if (m_CurrentTime >= m_Keys[i]->GetTime()){ m_CurrentKey = i; }
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

	if ((m_CurrentKey + 1) >= m_Keys.size() && !IsCycle() && !IsReverse())
	{
		m_Position = m_Keys[m_CurrentKey]->GetCameraInfo()->GetEye();
		m_Position = m_RotationOffset*(m_Position - m_PositionOffsetKey) + m_PositionOffset;
		m_Fov = m_Keys[m_CurrentKey]->GetCameraInfo()->GetFOV();
		return;
	}
		
	
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

		l_tI = m_Keys[m_CurrentKey]->GetTime();
		l_tF = m_Keys[m_NextKey]->GetTime();

		l_pI = m_Keys[m_CurrentKey]->GetCameraInfo()->GetEye();
		l_pF = m_Keys[m_NextKey]->GetCameraInfo()->GetEye();

		l_fI = m_Keys[m_CurrentKey]->GetCameraInfo()->GetFOV();
		l_fF = m_Keys[m_NextKey]->GetCameraInfo()->GetFOV();

		l_lI = m_Keys[m_CurrentKey]->GetCameraInfo()->GetLookAt();
		l_lF = m_Keys[m_NextKey]->GetCameraInfo()->GetLookAt();

		l_uI = m_Keys[m_CurrentKey]->GetCameraInfo()->GetUp();
		l_uF = m_Keys[m_NextKey]->GetCameraInfo()->GetUp();
	}
	else
	{
		l_CurrentTime = m_TotalTime - m_CurrentTime;

		l_tI = m_Keys[m_NextKey]->GetTime();
		l_tF = m_Keys[m_CurrentKey]->GetTime();

		l_pI = m_Keys[m_NextKey]->GetCameraInfo()->GetEye();
		l_pF = m_Keys[m_CurrentKey]->GetCameraInfo()->GetEye();

		l_fI = m_Keys[m_NextKey]->GetCameraInfo()->GetFOV();
		l_fF = m_Keys[m_CurrentKey]->GetCameraInfo()->GetFOV();

		l_lI = m_Keys[m_NextKey]->GetCameraInfo()->GetLookAt();
		l_lF = m_Keys[m_CurrentKey]->GetCameraInfo()->GetLookAt();

		l_uI = m_Keys[m_NextKey]->GetCameraInfo()->GetUp();
		l_uF = m_Keys[m_CurrentKey]->GetCameraInfo()->GetUp();
	}

	m_Position = (((l_pF - l_pI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_pI;
	m_Position = m_RotationOffset*(m_Position - m_PositionOffsetKey) + m_PositionOffset;
	m_Fov = (((l_fF - l_fI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_fI;
	m_LookAt = (((l_lF - l_lI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_lI;
	m_LookAt = m_RotationOffset*(m_LookAt - m_PositionOffsetKey) + m_PositionOffset;
	Vect3f l_Forward = (m_LookAt - m_Position).GetNormalized();
	m_Up = (((l_uF - l_uI)*(l_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_uI;
	m_Up = m_RotationOffset * m_Up;
	Quatf l_auxRotation = Quatf();
	l_auxRotation.SetFromFwdUp(l_Forward, m_Up);
	m_Rotation = l_auxRotation;
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
	Camera->SetPosition(m_Position);
	//Camera->SetPosition(Vect3f(0, 1.8, 0));
	Camera->SetLookAt(m_LookAt);
	//Camera->SetLookAt(m_RotationOffset * (m_LookAt - m_PositionOffsetKey) + Vect3f(0, 1.8, 0));
	Camera->SetUp(m_Up);
	Camera->SetMatrixs();
}

Vect3f CCameraKeyController::GetLastLookAt()
{
	return m_Keys[m_Keys.size()-1]->GetCameraInfo()->GetLookAt();
}

void CCameraKeyController::SetFirstKey(Vect3f _Forward, Vect3f _Up, float _Fov)
{
	Vect3f l_LookAt = m_Keys[0]->GetCameraInfo()->GetLookAt();
	m_Fov = _Fov;
	m_LookAt.y += _Forward.y;
	l_LookAt.y += _Forward.y;
	//m_Up = _Up;
	m_Keys[0]->GetCameraInfo()->SetFOV(_Fov);
	m_Keys[0]->GetCameraInfo()->SetLookAt(l_LookAt);
	//m_Keys[0]->GetCameraInfo()->SetUp(_Up);
}

CCameraInfo* CCameraKeyController::GetLastKey()
{
	return m_Keys[m_Keys.size()-1]->GetCameraInfo();
}

CCameraInfo CCameraKeyController::GetCameraAsInfo()
{
	return CCameraInfo(m_Position, m_LookAt, m_Up, m_Fov);
}

void CCameraKeyController::ForceUpdateYaw(float _ElapsedTime)
{
	CInputManager* l_InputManager = UABEngine.GetInputManager();
	float l_Radians = l_InputManager->GetAxisX()*_ElapsedTime;
	Vect3f l_ASD = m_Rotation.EulerFromQuat();
	float l_Yaw = m_Rotation.EulerFromQuat().z;
	//				15						165										-15					-165
	if (((l_Yaw < 0.261799f || l_Yaw > 2.87979f) && l_Radians < .0f) || ((l_Yaw > -0.261799f || l_Yaw < -2.87979f) && l_Radians > .0f))
	{
		Quatf l_YawRotation = Quatf(0, 0, 0, 1);
		l_YawRotation.SetFromScaledAxis(Vect3f(0, l_Radians, 0));
		m_Rotation = m_Rotation*l_YawRotation;
		l_InputManager->UpdateAxis(0, 0);
		m_LookAt = m_Position + GetForward();
	}
}