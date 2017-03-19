#ifndef CAMERA_INFO_H
#define CAMERA_INFO_H

#include "Utils.h"
#include "Math\Vector3.h"
#include "XML\tinyxml2.h"

class CCameraInfo
{	
public:
	CCameraInfo();
	CCameraInfo(const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, /*float NearPlane, float FarPlane,*/ float FOV);
	CCameraInfo(tinyxml2::XMLElement* TreeNode);
	virtual ~CCameraInfo();

	//UAB_BUILD_GET_SET(float, NearPlane);
	//UAB_BUILD_GET_SET(float, FarPlane);
	UAB_BUILD_GET_SET(float, FOV);
	UAB_BUILD_GET_SET(Vect3f, Eye);
	UAB_BUILD_GET_SET(Vect3f, LookAt);
	UAB_BUILD_GET_SET(Vect3f, Up);
};

#endif
