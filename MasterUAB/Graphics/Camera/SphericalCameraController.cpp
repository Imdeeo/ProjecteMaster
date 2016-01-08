#include <math.h>
#include <stdio.h>
#include "Camera\SphericalCameraController.h"
#include "Camera\Camera.h"
#include "Utils.h"

#include "Engine\UABEngine.h"

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
	Camera->SetFOV(1.047f);
	Camera->SetAspectRatio(UABEngine.GetRenderManager()->GetContextManager()->GetAspectRatio());
	Camera->SetZNear(0.1f);
	Camera->SetZFar(100.f);

	Camera->SetMatrixs();
}
