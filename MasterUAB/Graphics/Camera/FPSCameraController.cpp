#include "Camera\FPSCameraController.h"
#include "Camera\CameraInfo.h"
#include "Camera\Camera.h"

#include "Engine\UABEngine.h"

#include "InputManager\InputManager.h"

#include "Layers\LayerManager.h"

#include "LevelManager\LevelManager.h"

#include <string>

CFPSCameraController::CFPSCameraController(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) :CCameraController(TreeNode,_LevelId)
, m_Locked(false)
, m_YawSpeed(1.0f)
, m_PitchSpeed(0.5f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
, m_Target(UABEngine.GetLevelManager()->GetResource("Player")->GetLayerManager()->GetResource(TreeNode->GetPszProperty("layer"))->GetResource(TreeNode->GetPszProperty("target")))
, m_Offset(Vect3f(.0f, TreeNode->GetFloatProperty("offset", .0f), .0f))
{
	m_Rotation.SetFromAngleAxis(m_Offset, 0);
	m_Position = m_Target->GetPosition() + m_Offset;
	Quatf l_RotationXZ;
	Quatf l_RotationY;
	Quatf l_InitialRotation;
	l_InitialRotation = m_Target->GetRotation();
	l_InitialRotation.decoupleY(&l_RotationXZ, &l_RotationY);
	m_Rotation = l_RotationY;
}

CFPSCameraController::~CFPSCameraController()
{	
}

/*void CFPSCameraController::Move(float Strafe, float Forward, bool Speed, float ElapsedTime)
{
	Vect3f l_AddPos;
	float l_Yaw = m_Rotation.GetYaw();
	float l_Pitch = m_Rotation.GetPitch();

	l_AddPos.y = Forward*(sin(l_Pitch));
	l_AddPos.x = Forward*(cos(l_Yaw)) + Strafe*(cos(l_Yaw + FLOAT_PI_VALUE*0.5f));
	l_AddPos.z = Forward*(sin(l_Yaw)) + Strafe*(sin(l_Yaw + FLOAT_PI_VALUE*0.5f));
	
	float l_ConstantSpeed=ElapsedTime*m_Speed;
	if(Speed)
		l_ConstantSpeed*=m_FastSpeed;

	if (l_AddPos.SquaredLength() > 0)
	{
		l_AddPos.Normalize();
		l_AddPos *= l_ConstantSpeed;
		m_Position += l_AddPos;
	}
}*/

void CFPSCameraController::AddYaw(float Radians)
{
	CCameraController::AddYaw(Radians*m_YawSpeed);
}

void CFPSCameraController::AddPitch(float Radians)
{ 
	//float l_Pitch = m_Rotation.EulerFromQuat().x;
	//				80�						100�										-45�					-135�
	//if (((l_Pitch < 1.39626f || l_Pitch > 1.74533f) && Radians < .0f) || ((l_Pitch > -0.785398f || l_Pitch < -2.356194f) && Radians > .0f))
	
	
	float l_ForwardY = m_Rotation.GetForwardVector().y;

	if (l_ForwardY >= -0.7 && l_ForwardY <= 0.7 || l_ForwardY > 0.7 && Radians > 0 || l_ForwardY < -0.7 && Radians < 0)
	{
		CCameraController::AddPitch(-Radians*m_PitchSpeed);
	}
//	UtilsLog("Forward: " + std::to_string(l_Forward.y));

}

/*
15� = 0.261799f
30� = 0.523599f
45� = 0.785398f
60� = 1.0472f
80� = 1.39626f
100� = 1.74533f
120� = 2.0944f
135� = 2.356194f
150� = 2.61799f
165� = 2.87979f
*/

void CFPSCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetForward();
	Camera->SetFOV(m_Fov); //0.628319f 1.13446f
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

void CFPSCameraController::Update(float ElapsedTime)
{
	m_Position = m_Target->GetPosition() + m_Offset;
	if (m_Locked)
		return;
	CInputManager* l_InputManager = UABEngine.GetInputManager();
	AddPitch(l_InputManager->GetAxisY()*ElapsedTime);
	AddYaw(l_InputManager->GetAxisX()*ElapsedTime);
	l_InputManager->UpdateAxis(0, 0);
}

void CFPSCameraController::CopyFromKeyCamera(CCameraInfo* _CameraInfo)
{
	m_Fov = _CameraInfo->GetFOV();
	Vect3f l_LookAt = _CameraInfo->GetLookAt() - _CameraInfo->GetEye();
	m_Rotation.SetFromFwdUp(l_LookAt, _CameraInfo->GetUp());
}

void CFPSCameraController::CopyFromCamera(CCameraController* _Camera)
{
	m_Fov = _Camera->GetFov();
	m_Rotation.SetFromFwdUp(_Camera->GetForward(), _Camera->GetUp());
}
