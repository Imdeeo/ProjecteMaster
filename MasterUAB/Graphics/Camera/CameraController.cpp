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
	l_YawRotation.SetFromScaledAxis(Vect3f(0, Radians, 0));
	m_Rotation = m_Rotation*l_YawRotation;
}

void CCameraController::AddPitch(float Radians)
{
	Quatf l_PitchRotation = Quatf(0, 0, 0, 1);
	l_PitchRotation.SetFromScaledAxis(Vect3f(Radians, 0, 0));
	m_Rotation = l_PitchRotation*m_Rotation;
}

Vect3f CCameraController::GetDirection() const
{
	return m_Rotation.GetForwardVector();
}