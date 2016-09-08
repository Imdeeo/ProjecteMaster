#ifndef CAMERA_KEY_CONTROLLER_H
#define CAMERA_KEY_CONTROLLER_H

#include "Camera\CameraController.h"
#include "XML\tinyxml2.h"
#include <vector>

class CCameraKey;
class CCameraInfo;
class CXMLTreeNode;
class CCameraKeyController : public CCameraController
{	
private:
	std::vector<CCameraKey *> m_Keys;
	size_t m_CurrentKey, m_NextKey;
	float m_CurrentTime, m_TotalTime;
	Vect3f m_LookAt, m_Up;
	bool m_Cycle, m_Reverse;
	float m_ReverseDirection;
	bool LoadXML(const std::string &FileName);
	void GetCurrentKey();
public:
	CCameraKeyController(tinyxml2::XMLElement* TreeNode);
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

	CCameraKey* GetCameraKey(int _index)
	{
		return m_Keys[_index];
	}

	Vect3f m_PositionOffset;
	Mat33f m_LookAtOffset;
};

#endif
