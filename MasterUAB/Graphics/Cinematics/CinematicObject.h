#ifndef H_CINEMATIC_OBJECT_H
#define H_CINEMATIC_OBJECT_H

#include "Math\Vector3.h"
#include "Math\Quatn.h"
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

	Vect3f m_PivotPosition;
	Quatf  m_PivotRotation;
	Vect3f m_PivotScale;

	float calculateBestAngle(float _Current, float _Next);
public:
	CCinematicObject(CXMLTreeNode &TreeNode);
	virtual ~CCinematicObject();
	bool IsOk();
	void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame);
	void Update(float _ElapsedTime);
	void OnRestartCycle();
	void GetCurrentKey();

	void SetPivotPosition(Vect3f _PivotPosition);
	void SetPivotRotation(Quatf _PivotRotation);
	void SetPivotScale(Vect3f _PivotScale);

	Vect3f GetPivotPosition();
	Quatf GetPivotRotation();
	Vect3f GetPivotScale();
};

#endif H_CINEMATIC_OBJECT_H