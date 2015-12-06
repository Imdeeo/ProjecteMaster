#include "SpotLight.h"

#include "XML\XMLTreeNode.h"

CSpotLight::CSpotLight(){}

CSpotLight::CSpotLight(CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	SetAngle(TreeNode.GetFloatProperty("angle"));
	SetFallOff(TreeNode.GetFloatProperty("fall_off"));
}