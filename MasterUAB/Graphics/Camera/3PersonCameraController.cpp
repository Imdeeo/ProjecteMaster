#include "Camera\3PersonCameraController.h"
#include "Camera\Camera.h"

#include "Engine\UABEngine.h"

#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "InputManager\InputManager.h"

#include "Layers\LayerManager.h"


#define _USE_MATH_DEFINES
#include <math.h>

C3PersonCameraController::C3PersonCameraController(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CCameraController(TreeNode,_LevelId)
, m_YawSpeed(5.f)
, m_PitchSpeed(10.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
, m_Target(UABEngine.GetLayerManager()->GetResource(TreeNode->GetPszProperty("layer"))->GetResource(TreeNode->GetPszProperty("target")))
, m_PositionOffset(Vect3f(.0f, TreeNode->GetFloatProperty("position_offset", .0f), .0f))
, m_RotationOffset(TreeNode->GetVect3fProperty("rotation_offset", Vect3f(.0f, .0f, .0f)))
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
	Camera->SetFOV(m_Fov);
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
	
	if (UABEngine.GetInputManager()->GetMap()->GetBool(CInputManager::Actions::RightClick))
	{
		float l_Yaw = UABEngine.GetInputManager()->GetMap()->GetFloatDelta(CInputManager::Actions::AxisX) * 2.f;
		float l_Pitch = UABEngine.GetInputManager()->GetMap()->GetFloatDelta(CInputManager::Actions::AxisY) * -0.5f;
		AddYaw(l_Yaw*m_YawSpeed);
		AddPitch(l_Pitch*m_PitchSpeed);
	}
}
