#include "Camera\3PersonCameraController.h"
#include "Camera\Camera.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectsManager.h"

C3PersonCameraController::C3PersonCameraController(CXMLTreeNode &node)
: m_YawSpeed(100.f)
, m_PitchSpeed(60.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
{
	m_Position = node.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f), true);
	m_Target = UABEngine.GetLayerManager()->GetResource(node.GetPszProperty("layer"))->GetResource(node.GetPszProperty("target"));
	m_offset = m_Target->GetPosition() - m_Position;
}

C3PersonCameraController::~C3PersonCameraController()
{	
}

void C3PersonCameraController::Move(Vect3f _newPos, float ElapsedTime)
{	
	m_Position = _newPos;
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
	Camera->SetLookAt(m_Target->GetPosition());
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

Vect3f C3PersonCameraController::GetDirection() const
{
	return m_Target->GetPosition()-m_Position;
}

void C3PersonCameraController::Update(float ElapsedTime)
{
	Vect3f move = m_Target->GetPosition() + m_offset;
	Move(move, ElapsedTime);
}
