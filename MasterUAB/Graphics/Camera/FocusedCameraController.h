#ifndef INC_FOCUSEDCAMERA_H_
#define INC_FOCUSEDCAMERA_H_

#include "Camera\CameraController.h"
#include "XML\tinyxml2.h"

class CRenderableObject;

class CFocusedCameraController : public CCameraController
{
private:
	Vect3f						m_TargetPosition;
	Vect3f						m_Offset;
	Vect3f						m_Start;
	Vect3f						m_LookAt;
	CRenderableObject*			m_Player;
	float						m_Timer;
public:
	CFocusedCameraController(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CFocusedCameraController();
	
	void Init(Vect3f _Forward, Vect3f _Up, Vect3f _Focus, float _Fov);
	Vect3f GetLookAt();
	void SetStart(Vect3f _Start);
	void CopyToCamera(CCameraController* _Camera);
	void SetFocus(Vect3f _Target);
	void SetCamera(CCamera *Camera) const;
	void Update(float ElapsedTime);
};

#endif
