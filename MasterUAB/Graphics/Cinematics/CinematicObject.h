#ifndef H_CINEMATIC_OBJECT_H
#define H_CINEMATIC_OBJECT_H

#include "Cinematics\CinematicPlayer.h"
#include <stdlib.h>
#include <vector>

class CXMLTreeNode;
class CCinematicObjectKeyFrame;
class CRenderableObject;

class CCinematicObject : public CCinematicPlayer
{
private:
	std::vector<CCinematicObjectKeyFrame *> m_CinematicObjectKeyFrames;
	size_t m_CurrentKeyFrame;
	CRenderableObject *m_RenderableObject;

	float calculateBestAngle(float _Current, float _Next);
public:
	CCinematicObject(CXMLTreeNode &TreeNode);
	virtual ~CCinematicObject();
	bool IsOk();
	void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame);
	void Update(float _ElapsedTime);
	void OnRestartCycle();
	void GetCurrentKey();
};

#endif H_CINEMATIC_OBJECT_H