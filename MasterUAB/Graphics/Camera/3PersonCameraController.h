#ifndef H_3PERSON_CAMERA_CONTROLLER_H
#define H_3PERSON_CAMERA_CONTROLLER_H

#include "Camera\CameraController.h"
#include "RenderableObjects\RenderableObject.h"

class CCamera;

class C3PersonCameraController : public CCameraController
{
private:
	float						m_YawSpeed;
	float						m_PitchSpeed;
	float						m_Speed;
	float						m_FastSpeed;
	CRenderableObject			m_Target;
public:
	C3PersonCameraController(CXMLTreeNode &node);
	virtual ~C3PersonCameraController();
	
	void Move(float Strafe, float Forward, bool Speed, float ElapsedTime);
	void SetCamera(CCamera *Camera) const;
	void AddYaw(float Radians);
	void AddPitch(float Radians);
	Vect3f GetDirection() const;

};

#endif
