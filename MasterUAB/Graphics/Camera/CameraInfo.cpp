#include "CameraInfo.h"

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
<<<<<<< HEAD
	m_Eye = XMLTreeNode.GetVect3fProperty("pos", Vect3f(0, 0, 0));
	m_LookAt = XMLTreeNode.GetVect3fProperty("look_at", Vect3f(0, 0, 0));
	m_Up = XMLTreeNode.GetVect3fProperty("up", Vect3f(0, 0, 0));
	m_FOV = XMLTreeNode.GetFloatProperty("fox", 45);
	m_NearPlane = XMLTreeNode.GetFloatProperty("near_plane", 1000.0);
	m_FarPlane = XMLTreeNode.GetFloatProperty("far_plane", 1000.0);
=======
	std::string l_Eye;
	l_Eye = *XMLTreeNode.GetPszProperty("pos");
	std::string::size_type l_EyeSz;
	float l_EyeX = std::stof(l_Eye, &l_EyeSz);
	float l_EyeY = std::stof(l_Eye.substr(l_EyeSz), &l_EyeSz);
	float l_EyeZ = std::stof(l_Eye.substr(l_EyeSz));
	m_Eye = (l_EyeX, l_EyeY, l_EyeZ);

	std::string l_LookAt;
	l_LookAt = *XMLTreeNode.GetPszProperty("look_at");
	std::string::size_type l_LookAtSz;
	float l_LookAtX = std::stof(l_LookAt, &l_LookAtSz);
	float l_LookAtY = std::stof(l_LookAt.substr(l_LookAtSz), &l_LookAtSz);
	float l_LookAtZ = std::stof(l_LookAt.substr(l_LookAtSz));
	m_LookAt = (l_LookAtX, l_LookAtY, l_LookAtZ);

	std::string l_Up;
	l_Up = *XMLTreeNode.GetPszProperty("up");
	std::string::size_type l_UpSz;
	float l_UpX = std::stof(l_Up, &l_UpSz);
	float l_UpY = std::stof(l_Up.substr(l_UpSz), &l_UpSz);
	float l_UpZ = std::stof(l_Up.substr(l_UpSz));
	m_Up = (l_UpX, l_UpY, l_UpZ);
	m_Up = (0.0f, 1.0f, 0.0f);

	std::string l_NearPlane;
	l_NearPlane = *XMLTreeNode.GetPszProperty("near_plane");
	m_NearPlane = std::stof(l_NearPlane);
	
	std::string l_FarPlane;
	l_FarPlane = *XMLTreeNode.GetPszProperty("far_plane");
	m_FarPlane = std::stof(l_FarPlane);
	
	std::string l_FOV;
	l_FOV = *XMLTreeNode.GetPszProperty("fov");
	m_FOV = std::stof(l_FOV);
>>>>>>> develop
}

CCameraInfo::~CCameraInfo()
{
}
