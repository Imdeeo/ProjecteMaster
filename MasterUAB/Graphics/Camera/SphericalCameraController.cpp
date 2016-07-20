#include <math.h>
#include <stdio.h>
#include "Camera\SphericalCameraController.h"
#include "Camera\Camera.h"
#include "Utils.h"
#include "InputManager\InputManager.h"
#include "XML\XMLTreeNode.h"

CSphericalCameraController::CSphericalCameraController(const CXMLTreeNode & _TreeNode) : CCameraController(_TreeNode)
, m_Zoom(_TreeNode.GetFloatProperty("zoom", 50.f))
, m_ZoomSpeed(2.f)
, m_CameraPosition(m_Position - m_Zoom)
{
	m_Rotation.SetFromAngleAxis(m_Zoom, 0);
}

CSphericalCameraController::~CSphericalCameraController()
{	
}

void CSphericalCameraController::SetCamera(CCamera *Camera) const
{
	Camera->SetFOV(1.13446f);
	Camera->SetAspectRatio(16.0f / 9.0f);

	Camera->SetLookAt(m_Position);
	Camera->SetPosition(m_CameraPosition);

	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

void CSphericalCameraController::Update(float ElapsedTime)
{
	if (CInputManager::GetMap()->GetBool(CInputManager::Actions::RightClick))
	{
		Vect3f cameraMovement(0, 0, 0);
		cameraMovement.x = CInputManager::GetMap()->GetFloatDelta(CInputManager::Actions::AxisX);//->GetAxis("X_AXIS") * ElapsedTime * .5f;
		cameraMovement.y = CInputManager::GetMap()->GetFloatDelta(CInputManager::Actions::AxisY);//GetAxis("Y_AXIS") * ElapsedTime * -.5f;
		Rotate(cameraMovement);
	}
	if (CInputManager::GetMap()->GetBool(CInputManager::Actions::WheelUp))
	{
		m_Zoom -= 0.1f;
	}
	else if (CInputManager::GetMap()->GetBool(CInputManager::Actions::WheelDown))
	{
		m_Zoom += 0.1f;
	}
	if (m_Zoom < 1){ m_Zoom = 1; }
	m_CameraPosition = m_Position - (GetForward()*m_Zoom);
}