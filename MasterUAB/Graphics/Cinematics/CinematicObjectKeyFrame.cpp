#include "CinematicObjectKeyFrame.h"

#include "XML\XMLTreeNode.h"

CCinematicObjectKeyFrame::CCinematicObjectKeyFrame(CXMLTreeNode &TreeNode):C3DElement(TreeNode)
{
	SetKeyFrameTime(TreeNode.GetFloatProperty("time",0));
	
}

CCinematicObjectKeyFrame::~CCinematicObjectKeyFrame()
{
	
}