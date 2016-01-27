#include "Light.h"

#include "XML\XMLTreeNode.h"
//#include "RenderManager\RenderManager.h"

CLight::CLight() : CNamed(""),m_Type(LIGHT_TYPE_OMNI),m_Position(Vect3f(0.0f,0.0f,0.0f)),m_Color(Vect4f(1.0f,1.0f,1.0f,1.0f)),m_StartRangeAttenuation(0.0f),m_EndRangeAttenuation(0.0f),m_Intensity(0.0f),m_Enabled(false){}

CLight::CLight(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	m_Type = GetLightTypeByName(TreeNode.GetPszProperty("type"));
	m_Position = TreeNode.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f), true);
	m_Color = CColor(TreeNode.GetVect4fProperty("color", Vect4f(255.0f, 255.0f, 255.0f, 0.0f), true));
	m_StartRangeAttenuation = TreeNode.GetFloatProperty("att_start_range");
	m_EndRangeAttenuation = TreeNode.GetFloatProperty("att_end_range");
	m_Intensity = TreeNode.GetFloatProperty("intensity");
	m_Enabled = TreeNode.GetBoolProperty("enabled");
}

CLight::~CLight(){}

void CLight::Render(CRenderManager *RenderManager)
{
}

CLight::TLightType CLight::GetLightTypeByName(const std::string &StrLightType)
{
	if(StrLightType=="omni")
		return LIGHT_TYPE_OMNI;
	else if (StrLightType=="spot")
		return LIGHT_TYPE_SPOT;
	else if (StrLightType=="directional")
		return LIGHT_TYPE_DIRECTIONAL;
	return LIGHT_TYPE_NULL;
}