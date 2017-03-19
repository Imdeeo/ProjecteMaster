#include "Camera\FocusedCameraController.h"
#include "Camera\Camera.h"
#include "Utils.h"
#include "InputManager\InputManager.h"
#include "Layers\LayerManager.h"
#include "LevelManager\LevelManager.h"

CFocusedCameraController::CFocusedCameraController(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CCameraController(TreeNode, _LevelId)
, m_TargetPosition(Vect3f(.0f, .0f, .0f))
, m_Player(UABEngine.GetLevelManager()->GetResource("Player")->GetLayerManager()->GetLayer("solid")->GetResource("Jaheem"))
, m_Offset(Vect3f(.0f, TreeNode->GetFloatProperty("offset", .0f), .0f))
, m_Start(m_TargetPosition)
, m_Timer(.0f)
, m_LookAt(m_Start)
{
	m_Position = m_Player->GetPosition() + m_Offset;
	m_Rotation.SetFromFwdUp(GetLookAt(), Vect3f(.0f, 1.f, .0f));
}

CFocusedCameraController::~CFocusedCameraController()
{	
}

void CFocusedCameraController::Init(Vect3f _Forward, Vect3f _Up, Vect3f _Focus, float _Fov)
{
	m_Rotation.SetFromFwdUp(_Forward, _Up);
	m_Fov = _Fov;
	m_Start = _Forward;
	m_LookAt = m_Position + _Forward;
	m_Timer = .0f;
	m_TargetPosition = _Focus;
}

void CFocusedCameraController::SetFocus(Vect3f _Target)
{
	m_TargetPosition = _Target;
}

Vect3f CFocusedCameraController::GetLookAt()
{
	return m_TargetPosition - m_Position;
}

void CFocusedCameraController::SetStart(Vect3f _Start)
{
	m_Start = _Start;
	m_LookAt = m_Position + _Start;
	m_Timer = .0f;
}

void CFocusedCameraController::SetCamera(CCamera *Camera) const
{
	Camera->SetFOV(m_Fov);
	Camera->SetAspectRatio(16.0f / 9.0f);
	
	Camera->SetLookAt(m_LookAt);
	Camera->SetPosition(m_Position);

	Camera->SetUp(Vect3f(.0f, 1.f, .0f));
	Camera->SetMatrixs();
}

void CFocusedCameraController::Update(float ElapsedTime)
{
	Vect3f l_Up = GetUp();
	if (m_Timer <= 1.f){
		m_Timer += ElapsedTime;
		if (m_Timer > 1.f)
			m_Timer = 1.f;
		m_LookAt = (m_Start + m_Position).GetLerp(m_TargetPosition, m_Timer);
		l_Up.Lerp(Vect3f(.0f, 1.f, .0f), m_Timer);
		if (m_Timer == 1.f)
			m_Timer = 2.f;
	}
	m_Position = m_Player->GetPosition() + m_Offset;
	m_Rotation.SetFromFwdUp(m_LookAt - m_Position, l_Up);
}

void CFocusedCameraController::CopyToCamera(CCameraController* _Camera)
{
	_Camera->SetFov(m_Fov);
	_Camera->GetRotation().SetFromFwdUp(GetLookAt(), Vect3f(.0f, 1.f, .0f));
}
