#include "ManchasSystemType.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CManchasSystemType::CManchasSystemType(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	m_Material = UABEngine.GetInstance()->GetMaterialManager()->GetResource(l_Element.GetPszProperty("material"));
	m_Frames = l_Element.GetIntProperty("frames", 1);
	m_Size = l_Element.GetVect2fProperty("size",Vect2f(0.0, 0.0));
	m_SizeSpeed = l_Element.GetVect2fProperty("size_speed", Vect2f(0.0, 0.0));
	m_EmitTime = l_Element.GetVect2fProperty("emit_time", Vect2f(0.0, 0.0));
	m_Life = l_Element.GetVect2fProperty("life", Vect2f(0.0, 0.0));
	m_Opacity = l_Element.GetVect2fProperty("opacity", Vect2f(0.0, 0.0));
	m_Color1 = CColor(TreeNode.GetVect4fProperty("color_1", Vect4f(1.0, 1.0f, 1.0f, 1.0f)));
	m_Color2 = CColor(TreeNode.GetVect4fProperty("color_2", Vect4f(1.0f, 1.0f, 1.0f, 1.0f)));
}

CManchasSystemType::~CManchasSystemType(void)
{
	Destroy();
}

void CManchasSystemType::Destroy()
{
}

void CManchasSystemType::Save(FILE* _File)
{
	fprintf_s(_File, "\t<mancha name=\"%s\" material=\"%s\" frames=\"%i\" size=\"%f %f\" size_speed=\"%f %f\" life=\"%f %f\" "
		"emit_rate=\"%f %f\" opacity=\"%f %f\" color_1=\"%f %f %f %f\" color_2=\"%f %f %f %f\"/>\n",
		m_Name.c_str(), GetMaterial()->GetName().c_str(), m_Frames, m_Size[0], m_Size[1], m_SizeSpeed[0], m_SizeSpeed[1], m_Life[0], m_Life[1], 
		m_EmitTime[0], m_EmitTime[1], m_Opacity[0], m_Opacity[1], m_Color1.GetRed(), m_Color1.GetGreen(), m_Color1.GetBlue(), m_Color1.GetAlpha(),
		m_Color2.GetRed(), m_Color2.GetGreen(), m_Color2.GetBlue(), m_Color2.GetAlpha());
}