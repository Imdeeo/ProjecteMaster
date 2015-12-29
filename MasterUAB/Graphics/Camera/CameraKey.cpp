#include "CameraKey.h"

CCameraKey::CCameraKey(CCameraInfo &CameraInfo, float Time)
{
	m_CameraInfo = CameraInfo;
	m_Time = Time;
}

CCameraKey::~CCameraKey()
{
}
