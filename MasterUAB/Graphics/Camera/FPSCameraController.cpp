#include "Camera\FPSCameraController.h"
#include "Camera\Camera.h"

#include "Engine\UABEngine.h"

#include "InputManager\InputManager.h"
#include "XML\XMLTreeNode.h"

#include "Layers\LayerManager.h"

CFPSCameraController::CFPSCameraController(const CXMLTreeNode & _TreeNode) :CCameraController(_TreeNode)
, m_YawSpeed(2.5f)
, m_PitchSpeed(2.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
, m_Target(UABEngine.GetLayerManager()->GetResource(_TreeNode.GetPszProperty("layer"))->GetResource(_TreeNode.GetPszProperty("target")))
, m_Offset(Vect3f(.0f, _TreeNode.GetFloatProperty("offset", .0f, true), .0f))
{
	m_Rotation.SetFromAngleAxis(m_Offset, 0);
	m_Position = m_Target->GetPosition() + m_Offset;
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
	CCameraController::AddPitch(-Radians*m_PitchSpeed);
}

void CFPSCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetForward();
	Camera->SetFOV(1.13446f);
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

void CFPSCameraController::Update(float ElapsedTime)
{
	m_Position = m_Target->GetPosition() + m_Offset;
	AddYaw(CInputManager::GetInputManager()->GetAxis("X_AXIS") * ElapsedTime);
	AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime);
	//Move(CInputManager::GetInputManager()->GetAxis("STRAFE"), CInputManager::GetInputManager()->GetAxis("MOVE_FWD"), false, ElapsedTime);
}