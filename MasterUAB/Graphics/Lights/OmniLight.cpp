#include "OmniLight.h"
#include "Camera\Frustum.h"

#include <assert.h>

COmniLight::COmniLight():CLight(){}

COmniLight::COmniLight(std::string _name) : CLight(_name)
{
	m_Type = GetLightTypeByName("omni");
}

COmniLight::COmniLight(tinyxml2::XMLElement* TreeNode) : CLight(TreeNode){}

void COmniLight::SetShadowMap(CRenderManager &RenderManager)
{
	assert(!"this method must not be called");
}

bool const COmniLight::GetInsideFrustum()
{
	return m_Frustum->SphereVisible(m_Position, m_EndRangeAttenuation);
}

void COmniLight::Save(FILE* _File)
{
	fprintf_s(_File, "\t<light name=\"%s\" enabled=\"%s\" type=\"omni\" pos=\"%f %f %f\" color=\"%f %f %f %f\" att_start_range=\"%f\" att_end_range=\"%f\" intensity=\"%f\"/>\n",
		m_Name.c_str(), m_Enabled ? "true" : "false", m_Position.x, m_Position.y, m_Position.z, m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha(), 
		m_StartRangeAttenuation, m_EndRangeAttenuation, m_Intensity);
}