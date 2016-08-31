#ifndef CAMERA_KEY_CONTROLLER_H
#define CAMERA_KEY_CONTROLLER_H

#include "Camera\CameraController.h"
#include <vector>

class CCameraKey;
class CCameraInfo;
class CXMLTreeNode;
class CCameraKeyController : public CCameraController
{	
private:
	std::vector<CCameraKey *> m_Keys;
	size_t m_CurrentKey, m_NextKey;
	float m_CurrentTime, m_TotalTime, m_FOV;
	Vect3f m_LookAt, m_Up;
	bool m_Cycle, m_Reverse;
	float m_ReverseDirection;
	bool LoadXML(const std::string &FileName);
	void GetCurrentKey();
public:
	CCameraKeyController(const CXMLTreeNode &XMLTreeNode);
	virtual ~CCameraKeyController();
	void Update(float ElapsedTime);
	void SetCurrentTime(float CurrentTime);
	void ResetTime();
	float GetTotalTime();

	bool IsCycle() const;
	void SetCycle(bool Cycle);

	bool IsReverse() const;
	void SetReverse(bool Reverse);
	void SetCamera(CCamera *Camera) const;

	Vect3f GetLastLookAt();
	void SetFirstKey(Vect3f _Forward, Vect3f _Pos, Vect3f _Up, float _Fov);
	CCameraInfo* GetLastKey();

	Vect3f m_PositionOffset;
};

#endif
