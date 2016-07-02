#include "Camera\FPSCameraController.h"
#include "Camera\Camera.h"

#include "Engine\UABEngine.h"

#include "InputManager\InputManager.h"
#include "XML\XMLTreeNode.h"

#include "Layers\LayerManager.h"

CFPSCameraController::CFPSCameraController(const CXMLTreeNode & _TreeNode) :CCameraController(_TreeNode)
, m_Locked(false)
, m_YawSpeed(2.5f)
, m_PitchSpeed(2.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
, m_Fov(1.13446f)
, m_Target(UABEngine.GetLayerManager()->GetResource(_TreeNode.GetPszProperty("layer"))->GetResource(_TreeNode.GetPszProperty("target")))
, m_Offset(Vect3f(.0f, _TreeNode.GetFloatProperty("offset", .0f, true), .0f))
{
	m_Rotation.SetFromAngleAxis(m_Offset, 0);
	m_Position = m_Target->GetPosition() + m_Offset;
	Quatf l_RotationXZ;
	Quatf l_RotationY;
	Quatf l_InitialRotation;
	l_InitialRotation = m_Target->GetRotation();
	l_InitialRotation.decoupleY(&l_RotationXZ, &l_RotationY);
	m_Rotation = l_RotationY;
	if (_TreeNode.GetPszProperty("target") == "Jaheem")
	{
		std::string l_AnimationName;
		float l_AnimationDuration;

		CXMLTreeNode l_XML;
		if (l_XML.LoadFile("Data\\camera_animation.xml"))
		{
			CXMLTreeNode l_Input = l_XML["camera_animations"];
			if (l_Input.Exists())
			{
				for (int i = 0; i < l_Input.GetNumChildren(); ++i)
				{
					CXMLTreeNode l_Element = l_Input(i);
					if (l_Element.GetName() == std::string("camera_animation"))
					{
						l_AnimationName = l_Element.GetPszProperty("name");
						l_AnimationDuration = l_Element.GetFloatProperty("duration");

						CCameraKeyController *l_CameraKeyController = new CCameraKeyController(l_Element, l_AnimationDuration);
						l_CameraKeyController->SetName(l_AnimationName);

						m_Animations->AddResource(l_AnimationName, l_CameraKeyController);
					}
				}
			}
		}
	}
}

CFPSCameraController::~CFPSCameraController()
{	
}

void CFPSCameraController::AddYaw(float Radians)
{
	CCameraController::AddYaw(Radians*m_YawSpeed);
}

void CFPSCameraController::AddPitch(float Radians)
{
	float l_Pitch = m_Rotation.EulerFromQuat().x;
	if (((l_Pitch < 1.39626f || l_Pitch > 1.74533f) && Radians < .0f) || ((l_Pitch > -0.785398f || l_Pitch < -2.356194f) && Radians > .0f))
		CCameraController::AddPitch(-Radians*m_PitchSpeed);
}

void CFPSCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetForward();
	Camera->SetFOV(m_Fov);
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

void CFPSCameraController::Update(float ElapsedTime)
{
	m_Position = m_Target->GetPosition() + m_Offset;
	if (m_Locked)
		return;
	AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS") * ElapsedTime);
	AddYaw(CInputManager::GetInputManager()->GetAxis("X_AXIS") * ElapsedTime);
}

CTemplatedMapManager<CCameraKeyController>* CFPSCameraController::GetAnimations()
{
	return m_Animations;
}
