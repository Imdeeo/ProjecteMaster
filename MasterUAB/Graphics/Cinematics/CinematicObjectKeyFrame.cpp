#include "CinematicObjectKeyFrame.h"

CCinematicObjectKeyFrame::CCinematicObjectKeyFrame(tinyxml2::XMLElement* TreeNode) :C3DElement(TreeNode)
{
	SetKeyFrameTime(TreeNode->GetFloatProperty("time",0));
	
}

CCinematicObjectKeyFrame::~CCinematicObjectKeyFrame()
{
	
}