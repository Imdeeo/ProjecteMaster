#ifndef CAMERA_KEY_H
#define CAMERA_KEY_H

#include "CameraInfo.h"
#include "Utils.h"

class CCameraKey
{	
public:
	CCameraKey(CCameraInfo &CameraInfo, float Time);
	virtual ~CCameraKey();

	UAB_BUILD_GET_SET(CCameraInfo*, CameraInfo);
	UAB_BUILD_GET_SET(float, Time);
};

#endif
