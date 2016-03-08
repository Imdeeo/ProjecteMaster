#include "CameraInfo.h"

#include "XML\XMLTreeNode.h"

CCameraInfo::CCameraInfo()
{
}

CCameraInfo::CCameraInfo(const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float NearPlane, float FarPlane, float FOV)
{
	m_Eye = Eye;
	m_LookAt = LookAt;
	m_Up = Up;
	m_NearPlane = NearPlane;
	m_FarPlane = FarPlane;
	m_FOV = FOV;
}

CCameraInfo::CCameraInfo(CXMLTreeNode &XMLTreeNode)
{
	m_Eye = XMLTreeNode.GetVect3fProperty("pos", Vect3f(0, 0, 0));
	m_LookAt = XMLTreeNode.GetVect3fProperty("look_at", Vect3f(0, 0, 0));
	m_Up = XMLTreeNode.GetVect3fProperty("up", Vect3f(0, 0, 0));
	m_FOV = XMLTreeNode.GetFloatProperty("fox", 45);
	m_NearPlane = XMLTreeNode.GetFloatProperty("near_plane", 800.0);
	m_FarPlane = XMLTreeNode.GetFloatProperty("far_plane", 1600.0);
}

CCameraInfo::~CCameraInfo()
{
}
