#include "Player.h"
#include "Camera\CameraControllerManager.h"
#include "XML\XMLTreeNode.h"

#include "Engine\UABEngine.h"

CPlayer::CPlayer(const CXMLTreeNode& _TreeNode) :CCharacter(_TreeNode)
{
	m_CameraControllerName = _TreeNode.GetPszProperty("camera_controller");

	m_CameraController = UABEngine.GetCameraControllerManager()->GetResource(m_CameraControllerName);
}

CPlayer::~CPlayer()
{
}

CCameraController* CPlayer::GetCameraController()
{
	return m_CameraController;
}

void CPlayer::SetCameraController(CCameraController* _CameraController)
{
	m_CameraController = _CameraController;
}

bool CPlayer::Refresh()
{
	m_CameraController = UABEngine.GetCameraControllerManager()->GetResource(m_CameraControllerName);

	CCharacter::Refresh();

	return true;
}