#include "Camera\FPSCameraController.h"
#include "Camera\CameraInfo.h"
#include "Camera\Camera.h"

#include "Engine\UABEngine.h"

#include "InputManager\InputManager.h"
#include "XML\XMLTreeNode.h"

#include "Layers\LayerManager.h"

CFPSCameraController::CFPSCameraController(const CXMLTreeNode & _TreeNode) :CCameraController(_TreeNode)
, m_Locked(false)
, m_YawSpeed(2.5f)
, m_PitchSpeed(2.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
, m_Target(UABEngine.GetLayerManager()->GetResource(_TreeNode.GetPszProperty("layer"))->GetResource(_TreeNode.GetPszProperty("target")))
, m_Offset(Vect3f(.0f, _TreeNode.GetFloatProperty("offset", .0f, true), .0f))
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
	float l_Pitch = m_Rotation.EulerFromQuat().x;
	if (((l_Pitch < 1.39626f || l_Pitch > 1.74533f) && Radians < .0f) || ((l_Pitch > -0.785398f || l_Pitch < -2.356194f) && Radians > .0f))
		CCameraController::AddPitch(-Radians*m_PitchSpeed);
}

void CFPSCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetForward();
	Camera->SetFOV(m_Fov); //0.628319f 1.13446f
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
	Vect3f l_LookAt = GetUp();
	UtilsLog("Cam Up: " + std::to_string(l_LookAt.x) + ", " + std::to_string(l_LookAt.y) + ", " + std::to_string(l_LookAt.z));
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
	m_Fov = _CameraInfo->m_FOV;
	Vect3f l_LookAt = _CameraInfo->m_LookAt;
	m_Rotation.SetFromAngleAxis((l_LookAt - m_Position).GetNormalized(), 0);
}
