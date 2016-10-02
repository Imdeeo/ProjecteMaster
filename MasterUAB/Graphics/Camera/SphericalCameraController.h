#ifndef INC_SPHERICALCAMERA_H_
#define INC_SPHERICALCAMERA_H_

#include "Camera\CameraController.h"
#include "XML\tinyxml2.h"

class CSphericalCameraController : public CCameraController
{
private:
	float						m_Zoom;
	float						m_ZoomSpeed;
	Vect3f						m_CameraPosition;
public:
<<<<<<< HEAD
	CSphericalCameraController(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
=======
	CSphericalCameraController(tinyxml2::XMLElement* _TreeNode);
>>>>>>> develop
	virtual ~CSphericalCameraController();

	void AddZoom(float Zoom) {m_Zoom+=Zoom;}
	void SetZoom(float Zoom) {m_Zoom=Zoom;}
	
	void SetCamera(CCamera *Camera) const;
	
	void Rotate(Vect3f movement)
	{
		AddPitch(movement.y*10.0f);
		AddYaw(movement.x*10.0f);
	}

	void Update(float ElapsedTime);
};

#endif
