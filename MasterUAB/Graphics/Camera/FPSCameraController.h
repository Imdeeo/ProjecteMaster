#ifndef INC_CAMERAFPSHOOTER_H_
#define INC_CAMERAFPSHOOTER_H_

#include "Camera\CameraController.h"

class CCamera;
class CCameraKeyController;
class CRenderableObject;

class CFPSCameraController : public CCameraController
{
private:
	bool						m_Locked;
	float						m_YawSpeed;
	float						m_PitchSpeed;
	float						m_Speed;
	float						m_FastSpeed;
	CRenderableObject*			m_Target;
	Vect3f						m_Offset;
public:
	CFPSCameraController(const CXMLTreeNode & _TreeNode);
	virtual ~CFPSCameraController();
	
	void SetCamera(CCamera *Camera) const;
	void AddYaw(float Radians);
	void AddPitch(float Radians);
	void Update(float ElapsedTime);
	void Lock(){ m_Locked = true; }
	void Unlock(){ m_Locked = false; }
	void CopyFromKeyCamera(CCameraKeyController* _CameraController);
};

#endif
