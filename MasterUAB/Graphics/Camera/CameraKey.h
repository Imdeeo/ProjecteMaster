#ifndef CAMERA_KEY_H
#define CAMERA_KEY_H

#include "CameraInfo.h"

class CCameraKey
{	
public:
	CCameraInfo m_CameraInfo;
	float m_Time;

	CCameraKey(CCameraInfo &CameraInfo, float Time);
	virtual ~CCameraKey();
};

#endif
