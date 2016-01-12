#include "CinematicObjectKeyFrame.h"

CCinematicObjectKeyFrame::CCinematicObjectKeyFrame(CXMLTreeNode &TreeNode):C3DElement(TreeNode)
{
	SetKeyFrameTime(TreeNode.GetFloatProperty("time",0));
	
}
