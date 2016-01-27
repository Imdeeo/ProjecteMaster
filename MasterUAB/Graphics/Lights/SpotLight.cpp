#include "SpotLight.h"

#include "XML\XMLTreeNode.h"

CSpotLight::CSpotLight():CDirectionalLight(),m_Angle(0.0f),m_FallOff(0.0f){}

CSpotLight::CSpotLight(CXMLTreeNode &TreeNode) : CDirectionalLight(TreeNode)
{
	m_Angle = TreeNode.GetFloatProperty("angle",1.f);
	m_FallOff = TreeNode.GetFloatProperty("fall_off", 1.2f);
}