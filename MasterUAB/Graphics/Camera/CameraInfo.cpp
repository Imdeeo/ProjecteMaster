#include "CameraInfo.h"


CCameraInfo::CCameraInfo()
{
}

CCameraInfo::CCameraInfo(const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up,/* float NearPlane, float FarPlane, */float FOV)
{
	m_Eye = Eye;
	m_LookAt = LookAt;
	m_Up = Up;
	//m_NearPlane = NearPlane;
	//m_FarPlane = FarPlane;
	m_FOV = FOV;
}

CCameraInfo::CCameraInfo(tinyxml2::XMLElement* TreeNode)
{
	m_Eye = TreeNode->GetVect3fProperty("pos", Vect3f(0, 0, 0));
	m_LookAt = TreeNode->GetVect3fProperty("look_at", Vect3f(0, 0, 0));
	m_Up = TreeNode->GetVect3fProperty("up", Vect3f(0, 0, 0));
	m_FOV = TreeNode->GetFloatProperty("fov", 1.13f);
	//m_NearPlane = TreeNode->GetFloatProperty("near_plane", 800.0);
	//m_FarPlane = TreeNode->GetFloatProperty("far_plane", 1600.0);
}

CCameraInfo::~CCameraInfo()
{
}
