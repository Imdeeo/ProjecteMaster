#include <math.h>
#include <stdio.h>
#include "Camera\SphericalCameraController.h"
#include "Camera\Camera.h"
#include "Utils.h"
#include "InputManager\InputManager.h"
#include "XML\XMLTreeNode.h"

CSphericalCameraController::CSphericalCameraController(const CXMLTreeNode & _TreeNode) : CCameraController(_TreeNode)
, m_Zoom(50.f)
, m_ZoomSpeed(2.f)
, m_Offset(_TreeNode.GetVect3fProperty("offset", Vect3f(-15.f, -15.f, -15.f)))
, m_CameraPosition(m_Position - m_Offset)
{
	m_Rotation.SetFromAngleAxis(m_Offset, 0);
}

CSphericalCameraController::~CSphericalCameraController()
{	
}

void CSphericalCameraController::SetCamera(CCamera *Camera) const
{
	Camera->SetLookAt(m_Position);

	Camera->SetPosition(m_CameraPosition);

	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

void CSphericalCameraController::Update(float ElapsedTime)
{
	if (CInputManager::GetInputManager()->IsActionActive("MOVE_CAMERA"))
	{
		m_CameraPosition = m_Position - (GetDirection()*m_Offset);

		Vect3f cameraMovement(0, 0, 0);
		cameraMovement.x = CInputManager::GetInputManager()->GetAxis("X_AXIS") * ElapsedTime * 0.5f;
		cameraMovement.y = CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime * 0.5f;
		Move(cameraMovement);
	}
}