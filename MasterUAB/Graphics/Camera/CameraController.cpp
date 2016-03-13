#include "CameraController.h"

#include <cmath>

#include "Utils.h"

#include "XML\XMLTreeNode.h"

CCameraController::CCameraController(const CXMLTreeNode & _TreeNode) : CNamed(_TreeNode)
, m_Rotation(Quatf(0, 0, 0, 1))
, m_Position(0,0,0)
{
}
CCameraController::~CCameraController()
{
}

Vect3f CCameraController::GetRight() const
{
	return m_Rotation.GetRightVector();
}

Vect3f CCameraController::GetUp() const
{
	return m_Rotation.GetUpVector();
}

void CCameraController::AddYaw(float Radians)
{
	Quatf l_YawRotation = Quatf(0, 0, 0, 1);
	l_YawRotation.SetFromAngleAxis(Vect3f(0, 1, 0), Radians);
	m_Rotation = m_Rotation*l_YawRotation;
}

void CCameraController::AddPitch(float Radians)
{
	Quatf l_pitchRotation = Quatf(0, 0, 0, 1);
	l_pitchRotation.SetFromAngleAxis(Vect3f(1, 0, 0), Radians);
	m_Rotation = l_pitchRotation*m_Rotation;
}

Vect3f CCameraController::GetDirection() const
{
	return m_Rotation.GetAxis();
}