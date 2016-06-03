#include "OmniLight.h"

#include <assert.h>
#include "XML\XMLTreeNode.h"

COmniLight::COmniLight():CLight(){}

COmniLight::COmniLight(CXMLTreeNode &TreeNode) : CLight(TreeNode){}

void COmniLight::SetShadowMap(CRenderManager &RenderManager)
{
	assert(!"this method must not be called");
}

void COmniLight::Save(FILE* _File)
{
	fprintf_s(_File, "\t<light name=\"%s\" enabled=\"%s\" type=\"omni\" pos=\"%f %f %f\" color=\"%f %f %f %f\" att_start_range=\"%f\" att_end_range=\"%f\" intensity=\"%f\"/>\n",
		m_Name.c_str(), m_Enabled ? "true" : "false", m_Position.x, m_Position.y, m_Position.z, m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha(), 
		m_StartRangeAttenuation, m_EndRangeAttenuation, m_Intensity);
}