#include <math.h>
#include <stdio.h>
#include "Camera\SphericalCameraController.h"
#include "Camera\Camera.h"
#include "Utils.h"
#include "InputManager\InputManager.h"

CSphericalCameraController::CSphericalCameraController() 
: m_Zoom(50.f)
, m_ZoomSpeed(2.f)
{
}

CSphericalCameraController::~CSphericalCameraController()
{	
}

Vect3f CSphericalCameraController::GetDirection() const
{
	return Vect3f(m_Zoom*cos(m_Yaw)*cos(m_Pitch), m_Zoom*sin(m_Pitch), m_Zoom*sin(m_Yaw)*cos(m_Pitch));
}

void CSphericalCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();

	Camera->SetLookAt(m_Position);

	Camera->SetPosition(m_Position-l_Direction);

	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

void CSphericalCameraController::Update(float ElapsedTime)
{
	if (CInputManager::GetInputManager()->IsActionActive("MOVE_CAMERA"))
	{
		Vect3f cameraMovement(0, 0, 0);
		cameraMovement.x += CInputManager::GetInputManager()->GetAxis("X_AXIS") * ElapsedTime * 0.5f;
		cameraMovement.y += CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime * 0.5f;
		Move(cameraMovement);
	}
}