#ifndef H_CINEMATIC_OBJECT_KEY_FRAME_H
#define H_CINEMATIC_OBJECT_KEY_FRAME_H

#include "3DElement\3DElement.h"
#include "Utils.h"
#include "XML\XMLTreeNode.h"

class CCinematicObjectKeyFrame : public C3DElement
{

public:
	CCinematicObjectKeyFrame(CXMLTreeNode &TreeNode);
	virtual ~CCinematicObjectKeyFrame();

	UAB_BUILD_GET_SET(float,KeyFrameTime);
};

#endif H_CINEMATIC_OBJECT_KEY_FRAME_H