#ifndef INC_SPHERICALCAMERA_H_
#define INC_SPHERICALCAMERA_H_

#include "CameraController.h"

class CSphericalCameraController : public CCameraController
{
private:
	float						m_Zoom;
	float						m_ZoomSpeed;
public:
	CSphericalCameraController();
	virtual ~CSphericalCameraController();

	void AddZoom(float Zoom) {m_Zoom+=Zoom;}
	void SetZoom(float Zoom) {m_Zoom=Zoom;}
	
	void SetCamera(CCamera *Camera) const;
	Vect3f GetDirection() const;

	void Update(Vect3f movement)
	{
		AddYaw(movement.x*30.0f);
		AddPitch(movement.y*30.0f);
		AddZoom(-movement.z*2.0f);
	}
};

#endif
