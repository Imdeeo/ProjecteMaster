#ifndef H_CINEMATIC_OBJECT_H
#define H_CINEMATIC_OBJECT_H

#include "Math\Vector3.h"
#include "Math\Quatn.h"
#include "Cinematics\CinematicPlayer.h"
#include <stdlib.h>
#include <vector>
#include "XML\tinyxml2.h"

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
	CCinematicObject(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CCinematicObject();
	bool IsOk();
	void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame);
	void Update(float _ElapsedTime);
	void OnRestartCycle();
	void GetCurrentKey();
	bool IsFinished();
};

#endif H_CINEMATIC_OBJECT_H