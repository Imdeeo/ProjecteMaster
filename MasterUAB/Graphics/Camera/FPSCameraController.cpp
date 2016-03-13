#include "Camera\FPSCameraController.h"
#include "Camera\Camera.h"
#include "InputManager\InputManager.h"
#include "XML\XMLTreeNode.h"

CFPSCameraController::CFPSCameraController(const CXMLTreeNode & _TreeNode) :CCameraController(_TreeNode)
, m_YawSpeed(100.f)
, m_PitchSpeed(60.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
{
	m_Position=Vect3f(0.0f, 2.0f, 0.0f);
}

CFPSCameraController::~CFPSCameraController()
{	
}

void CFPSCameraController::Move(float Strafe, float Forward, bool Speed, float ElapsedTime)
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
}

void CFPSCameraController::AddYaw(float Radians)
{
	CCameraController::AddYaw(-Radians*m_YawSpeed);
}

void CFPSCameraController::AddPitch(float Radians)
{
	CCameraController::AddPitch(Radians*m_PitchSpeed);
}

void CFPSCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();
	Camera->SetFOV(1.13446f);
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

void CFPSCameraController::Update(float ElapsedTime)
{
	AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * ElapsedTime * 0.05f);
	AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime * 0.5f);
	Move(CInputManager::GetInputManager()->GetAxis("STRAFE"), CInputManager::GetInputManager()->GetAxis("MOVE_FWD"), false, ElapsedTime);
}