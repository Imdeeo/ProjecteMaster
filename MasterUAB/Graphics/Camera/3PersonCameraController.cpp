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
, m_YawSpeed(5.f)
, m_PitchSpeed(10.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
, m_Target(UABEngine.GetLayerManager()->GetResource(_TreeNode.GetPszProperty("layer"))->GetResource(_TreeNode.GetPszProperty("target")))
, m_PositionOffset(Vect3f(.0f, _TreeNode.GetFloatProperty("position_offset", .0f, true), .0f))
, m_RotationOffset(_TreeNode.GetVect3fProperty("rotation_offset", Vect3f(.0f, .0f, .0f), true))
{
	m_Rotation.SetFromAngleAxis(m_RotationOffset, 0);
	m_Position = m_Target->GetPosition() + m_PositionOffset + (GetForward()*m_RotationOffset.x) + (GetRight()*m_RotationOffset.z);
}

C3PersonCameraController::~C3PersonCameraController()
{	
}

void C3PersonCameraController::Move(Vect3f _MovementVector, float ElapsedTime)
{	
	m_Position = m_Position + _MovementVector;
}

void C3PersonCameraController::SetCamera(CCamera *Camera) const
{
	Camera->SetFOV(1.13446f);
	Camera->SetAspectRatio(16.0f/9.0f);

	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position + GetForward());
	
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

Vect3f C3PersonCameraController::GetDirection() const
{	
	return  m_Target->GetPosition() - m_Position;
}

void C3PersonCameraController::Update(float ElapsedTime)
{
	m_Position = m_Target->GetPosition() + m_PositionOffset + (GetForward()*m_RotationOffset.x) + (GetRight()*m_RotationOffset.z);

	if (CInputManager::GetInputManager()->IsActionActive("MOVE_CAMERA"))
	{
		float l_Yaw = CInputManager::GetInputManager()->GetAxis("X_AXIS") * ElapsedTime * 2.f;
		float l_Pitch = CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime * -0.5f;
		AddYaw(l_Yaw*m_YawSpeed);
		AddPitch(l_Pitch*m_PitchSpeed);
	}
}
