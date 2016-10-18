#include "ManchasSystemType.h"
#include "Engine\UABEngine.h"

#include "LevelManager\LevelManager.h"

CManchasSystemType::CManchasSystemType(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CNamed(TreeNode), CLevelInfo(_LevelId)
{
	m_Material = UABEngine.GetLevelManager()->GetResource(_LevelId)->GetMaterialManager()->GetResource(TreeNode->GetPszProperty("material"));
	m_Frames = TreeNode->GetIntProperty("frames", 1);
	m_Size = TreeNode->GetVect2fProperty("size", Vect2f(0.0, 0.0));
	m_SizeSpeed = TreeNode->GetVect2fProperty("size_speed", Vect2f(0.0, 0.0));
	m_EmitTime = TreeNode->GetVect2fProperty("emit_time", Vect2f(0.0, 0.0));
	m_Life = TreeNode->GetVect2fProperty("life", Vect2f(0.0, 0.0));
	m_Opacity = TreeNode->GetVect2fProperty("opacity", Vect2f(0.0, 0.0));
	m_Color1 = CColor(TreeNode->GetVect4fProperty("color_1", Vect4f(1.0, 1.0f, 1.0f, 1.0f)));
	m_Color2 = CColor(TreeNode->GetVect4fProperty("color_2", Vect4f(1.0f, 1.0f, 1.0f, 1.0f)));
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
		"emit_time=\"%f %f\" opacity=\"%f %f\" color_1=\"%f %f %f %f\" color_2=\"%f %f %f %f\"/>\n",
		m_Name.c_str(), GetMaterial()->GetName().c_str(), m_Frames, m_Size[0], m_Size[1], m_SizeSpeed[0], m_SizeSpeed[1], m_Life[0], m_Life[1], 
		m_EmitTime[0], m_EmitTime[1], m_Opacity[0], m_Opacity[1], m_Color1.GetRed(), m_Color1.GetGreen(), m_Color1.GetBlue(), m_Color1.GetAlpha(),
		m_Color2.GetRed(), m_Color2.GetGreen(), m_Color2.GetBlue(), m_Color2.GetAlpha());
}