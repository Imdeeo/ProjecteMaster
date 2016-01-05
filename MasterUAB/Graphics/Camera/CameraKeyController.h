#ifndef CAMERA_KEY_CONTROLLER_H
#define CAMERA_KEY_CONTROLLER_H

#include "XML\XMLTreeNode.h"
#include "CameraKey.h"
#include <vector>

class CCameraKeyController
{	
private:
	std::vector<CCameraKey *> m_Keys;
	size_t m_CurrentKey, m_NextKey;
	float m_CurrentTime, m_TotalTime;
	bool m_Cycle, m_Reverse;
	bool LoadXML(const std::string &FileName);
	void GetCurrentKey();
public:
	CCameraKeyController(CXMLTreeNode &XMLTreeNode);
	virtual ~CCameraKeyController();
	
	void Update(float ElapsedTime);

	void SetCurrentTime(float CurrentTime);
	void ResetTime();
	float GetTotalTime();

	bool IsCycle() const;
	void SetCycle(bool Cycle);

	bool IsReverse() const;
	void SetReverse(bool Reverse);
};

#endif
