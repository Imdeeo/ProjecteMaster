#ifndef CAMERA_INFO_H
#define CAMERA_INFO_H

#include "Math\Vector3.h"
#include "XML\tinyxml2.h"


class CCameraInfo
{	
public:
	float m_NearPlane, m_FarPlane, m_FOV;
	Vect3f m_Eye, m_LookAt, m_Up;

	CCameraInfo();
	CCameraInfo(const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float NearPlane, float FarPlane, float FOV);
	CCameraInfo(tinyxml2::XMLElement* TreeNode);
	virtual ~CCameraInfo();
};

#endif
