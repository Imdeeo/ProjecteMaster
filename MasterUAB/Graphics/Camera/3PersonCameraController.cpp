#include "Camera\3PersonCameraController.h"
#include "Camera\Camera.h"

#include "Engine\UABEngine.h"

#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "InputManager\InputManager.h"

#include "Layers\LayerManager.h"

#include "XML\XMLTreeNode.h"

#define _USE_MATH_DEFINES
#include <math.h>

C3PersonCameraController::C3PersonCameraController(const CXMLTreeNode & _TreeNode) : CCameraController(_TreeNode)
, m_YawSpeed(100.f)
, m_PitchSpeed(60.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
{
	m_Offset = _TreeNode.GetVect3fProperty("offset", Vect3f(0.0f, 0.0f, 0.0f), true);
	m_Target = UABEngine.GetLayerManager()->GetResource(_TreeNode.GetPszProperty("layer"))->GetResource(_TreeNode.GetPszProperty("target"));
	m_Position = m_Target->GetPosition() - m_Offset;
	Vect2f zero = Vect2f(1, 0);
	Vect2f offset = Vect2f(m_Offset.x, m_Offset.z);
	m_distance = sqrtf(offset.x*offset.x + offset.y*offset.y);
	m_angle = acosf(zero*offset.GetNormalized());
}

C3PersonCameraController::~C3PersonCameraController()
{	
}

void C3PersonCameraController::Move(Vect3f _MovementVector, float ElapsedTime)
{	
	m_Position = m_Position + _MovementVector;
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
	Vect3f auxLookAt = m_Target->GetPosition();
	auxLookAt.y += 2;
	Camera->SetLookAt(auxLookAt);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

Vect3f C3PersonCameraController::GetDirection() const
{	
	return  m_Target->GetPosition() - m_Position;
}

void C3PersonCameraController::Update(float ElapsedTime)
{
	Vect3f cameraMovement(0, 0, 0);
	cameraMovement.x += CInputManager::GetInputManager()->GetAxis("X_AXIS") * ElapsedTime * 1.f;
	if (cameraMovement.x > 0)
	{
		int i = 1;
	}
	//cameraMovement.y += CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime * 0.5f;
	m_angle = m_angle + cameraMovement.x;
	while (m_angle > FLOAT_PI_VALUE)
	{
		m_angle = m_angle - FLOAT_PI_VALUE * 2;
	}
	while (m_angle < -FLOAT_PI_VALUE)
	{
		m_angle = m_angle + FLOAT_PI_VALUE * 2;
	}
	Vect2f l_2Doffset = Vect2f(cos(m_angle),sin(m_angle));
	l_2Doffset = l_2Doffset*m_distance;
	Vect3f l_offset = Vect3f(l_2Doffset.x, m_Offset.y, l_2Doffset.y);
	Vect3f l_NewPos = m_Target->GetPosition() - l_offset;
	Move(l_NewPos-m_Position, ElapsedTime);
}
