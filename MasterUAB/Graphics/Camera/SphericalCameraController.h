#ifndef INC_SPHERICALCAMERA_H_
#define INC_SPHERICALCAMERA_H_

#include "Camera\CameraController.h"

class CSphericalCameraController : public CCameraController
{
private:
	float						m_Zoom;
	float						m_ZoomSpeed;
	Vect3f						m_Offset;
	Vect3f						m_CameraPosition;
public:
	CSphericalCameraController(const CXMLTreeNode & _TreeNode);
	virtual ~CSphericalCameraController();

	void AddZoom(float Zoom) {m_Zoom+=Zoom;}
	void SetZoom(float Zoom) {m_Zoom=Zoom;}
	
	void SetCamera(CCamera *Camera) const;
	
	void Move(Vect3f movement)
	{
		/*Quatf l_Movement = Quatf();
		l_Movement.SetFromScaledAxis(movement);
		m_Rotation = m_Rotation*l_Movement;*/
		AddPitch(movement.y*30.0f);
		AddYaw(movement.x*30.0f);
	}

	void Update(float ElapsedTime);
};

#endif
