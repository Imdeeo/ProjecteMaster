#include <math.h>
#include <stdio.h>
#include "Camera\SphericalCameraController.h"
#include "Camera\Camera.h"
#include "Utils.h"
#include "InputManager\InputManager.h"

CSphericalCameraController::CSphericalCameraController(const CXMLTreeNode & _TreeNode) : CCameraController(_TreeNode)
, m_Zoom(_TreeNode.GetFloatProperty("zoom", 4.5f))
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
	Camera->SetFOV(m_Fov);
	Camera->SetAspectRatio(16.0f / 9.0f);

	Camera->SetLookAt(m_Position);
	Camera->SetPosition(m_CameraPosition);

	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

void CSphericalCameraController::Update(float ElapsedTime)
{
	if (UABEngine.GetInputManager()->GetMap()->GetBool(CInputManager::Actions::RightClick))
	{
		Vect3f cameraMovement(0, 0, 0);
		cameraMovement.x = UABEngine.GetInputManager()->GetMap()->GetFloatDelta(CInputManager::Actions::AxisX);
		cameraMovement.y = -UABEngine.GetInputManager()->GetMap()->GetFloatDelta(CInputManager::Actions::AxisY);
		Rotate(cameraMovement);
	}
	if (UABEngine.GetInputManager()->GetMap()->GetBool(CInputManager::Actions::Mouse5))
	{
		m_Zoom -= 0.1f;
	}
	else if (UABEngine.GetInputManager()->GetMap()->GetBool(CInputManager::Actions::Mouse6))
	{
		m_Zoom += 0.1f;
	}
	if (m_Zoom < 0.1f){ m_Zoom = 0.1f; }
	m_CameraPosition = m_Position - (GetForward()*m_Zoom);
}