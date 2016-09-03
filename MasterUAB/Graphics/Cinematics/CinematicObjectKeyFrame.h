#ifndef H_CINEMATIC_OBJECT_KEY_FRAME_H
#define H_CINEMATIC_OBJECT_KEY_FRAME_H

#include "3DElement\3DElement.h"
#include "XML\tinyxml2.h"

class CCinematicObjectKeyFrame : public C3DElement
{
private:

	float m_KeyFrameTime;

public:

	CCinematicObjectKeyFrame(tinyxml2::XMLElement* TreeNode);
	virtual ~CCinematicObjectKeyFrame();

	float GetKeyFrameTime(){ return m_KeyFrameTime; }
	void SetKeyFrameTime(float _KeyFrameTime){ m_KeyFrameTime = _KeyFrameTime; }
};

#endif H_CINEMATIC_OBJECT_KEY_FRAME_H