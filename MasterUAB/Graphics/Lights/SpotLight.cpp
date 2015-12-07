#include "SpotLight.h"

#include "XML\XMLTreeNode.h"

CSpotLight::CSpotLight(){}

CSpotLight::CSpotLight(CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	m_Angle = TreeNode.GetFloatProperty("angle");
	m_FallOff = TreeNode.GetFloatProperty("fall_off");
}