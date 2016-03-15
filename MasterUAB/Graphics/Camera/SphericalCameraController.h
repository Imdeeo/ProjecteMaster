#ifndef INC_SPHERICALCAMERA_H_
#define INC_SPHERICALCAMERA_H_

#include "Camera\CameraController.h"

class CSphericalCameraController : public CCameraController
{
private:
	float						m_Zoom;
	float						m_ZoomSpeed;
	float						m_Distance;
	Vect3f						m_CameraPosition;
public:
	CSphericalCameraController(const CXMLTreeNode & _TreeNode);
	virtual ~CSphericalCameraController();

	void AddZoom(float Zoom) {m_Zoom+=Zoom;}
	void SetZoom(float Zoom) {m_Zoom=Zoom;}
	
	void SetCamera(CCamera *Camera) const;
	
	void Move(Vect3f movement)
	{
		AddPitch(movement.y*10.0f);
		AddYaw(movement.x*10.0f);
	}

	void Update(float ElapsedTime);
};

#endif
