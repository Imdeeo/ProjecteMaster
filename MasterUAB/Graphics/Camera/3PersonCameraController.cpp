#include "Camera\3PersonCameraController.h"
#include "Camera\Camera.h"

C3PersonCameraController::C3PersonCameraController(CXMLTreeNode &node)
: m_YawSpeed(100.f)
, m_PitchSpeed(60.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
{
	m_Position = node.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f), true);

}

C3PersonCameraController::~C3PersonCameraController()
{	
}

void C3PersonCameraController::Move(float Strafe, float Forward, bool Speed, float ElapsedTime)
{	
	Vect3f l_AddPos;
	
	l_AddPos.y = Forward*(sin(m_Pitch));
	l_AddPos.x=Forward*(cos(m_Yaw))+Strafe*(cos(m_Yaw+3.14159f*0.5f));
	l_AddPos.z=Forward*(sin(m_Yaw))+Strafe*(sin(m_Yaw+3.14159f*0.5f));
	
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

void C3PersonCameraController::AddYaw(float Radians)
{
	CCameraController::AddYaw(-Radians*m_YawSpeed);
}

void C3PersonCameraController::AddPitch(float Radians)
{
	CCameraController::AddPitch(Radians*m_PitchSpeed);
}

void C3PersonCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();
	Camera->SetFOV(0.87266f);
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

Vect3f C3PersonCameraController::GetDirection() const
{
	Vect3f l_Direction(cos(m_Yaw)*cos(m_Pitch), sin(m_Pitch), sin(m_Yaw)*cos(m_Pitch));
	return l_Direction;
}
