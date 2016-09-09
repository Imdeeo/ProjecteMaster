#ifndef H_3PERSON_CAMERA_CONTROLLER_H
#define H_3PERSON_CAMERA_CONTROLLER_H

#include "Camera\CameraController.h"
#include "XML\tinyxml2.h"

class CCamera;
class CRenderableObject;
class CInputManager;

class C3PersonCameraController : public CCameraController
{
private:
	float						m_YawSpeed;
	float						m_PitchSpeed;
	float						m_Speed;
	float						m_FastSpeed;
	CRenderableObject*			m_Target;
	float						m_Angle;
	Vect3f						m_PositionOffset;
	Vect3f						m_RotationOffset;
public:
	C3PersonCameraController(tinyxml2::XMLElement* TreeNode);
	virtual ~C3PersonCameraController();
	
	void Move(Vect3f _newPos, float ElapsedTime);
	void SetCamera(CCamera *Camera) const;
	Vect3f GetDirection() const;
	void Update(float ElapsedTime);
};

#endif
