#include "ParticleSystemType.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CParticleSystemType::CParticleSystemType(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	m_Material = UABEngine.GetInstance()->GetMaterialManager()->GetResource(l_Element.GetPszProperty("material"));
	m_NumFrames = l_Element.GetIntProperty("frames",1);
	m_TimePerFrame = l_Element.GetFloatProperty("time",1.0f);
	m_LoopFrames = l_Element.GetBoolProperty("loop",false);
	m_EmitAbsolute = l_Element.GetBoolProperty("emit_absolute",false);
	m_StartingDirectionAngle = l_Element.GetFloatProperty("start_dir_angle",1.0);
	m_StartingAccelerationAngle = l_Element.GetFloatProperty("start_acc_angle",1.0);
	m_Size = l_Element.GetVect2fProperty("size",Vect2f(0.0, 0.0));
	m_EmitRate = l_Element.GetVect2fProperty("emit_rate",Vect2f(0.0,0.0));
	m_AwakeTime = l_Element.GetVect2fProperty("awake_time", Vect2f(0.0, 0.0));
	m_SleepTime = l_Element.GetVect2fProperty("sleep_time", Vect2f(0.0, 0.0));
	m_Life = l_Element.GetVect2fProperty("life", Vect2f(0.0, 0.0));
	m_StartingAngle = l_Element.GetVect2fProperty("start_angle", Vect2f(0.0, 0.0));
	m_StartingAngularSpeed = l_Element.GetVect2fProperty("start_angular_speed", Vect2f(0.0, 0.0));
	m_AngularAcceleration = l_Element.GetVect2fProperty("angular_acc", Vect2f(0.0, 0.0));
	m_StartingSpeed1 = l_Element.GetVect3fProperty("start_speed_1", Vect3f(0.0, 0.0, 0.0));
	m_StartingSpeed2 = l_Element.GetVect3fProperty("start_speed_2", Vect3f(0.0, 0.0, 0.0));
	m_StartingAcceleration1 = l_Element.GetVect3fProperty("start_acc_1", Vect3f(0.0, 0.0, 0.0));
	m_StartingAcceleration2 = l_Element.GetVect3fProperty("start_acc_2", Vect3f(0.0, 0.0, 0.0));
	m_Color1 = CColor(TreeNode.GetVect4fProperty("color_1", Vect4f(1.0, 1.0f, 1.0f, 1.0f)));
	m_Color2 = CColor(TreeNode.GetVect4fProperty("color_2", Vect4f(1.0f, 1.0f, 1.0f, 1.0f)));

	for (int i = 0; i < l_Element.GetNumChildren(); ++i)
	{
		if (l_Element(i).GetPszProperty("type") == std::string("size"))
		{
			ControlPointSize aux;
			aux.m_Size = l_Element(i).GetVect2fProperty("size", Vect2f(0.0, 0.0));
			aux.m_Time = l_Element(i).GetVect2fProperty("time", Vect2f(0.0, 0.0));
			m_ControlPointSizes.push_back(aux);
		}
		else if (l_Element(i).GetPszProperty("type") == std::string("color"))
		{
			ControlPointColor aux;
			aux.m_Color1 = CColor(l_Element(i).GetVect4fProperty("color1", Vect4f(0.0, 0.0, 0.0, 1.0)));
			aux.m_Color2 = CColor(l_Element(i).GetVect4fProperty("color2", Vect4f(0.0, 0.0, 0.0, 1.0)));
			aux.m_Time = l_Element(i).GetVect2fProperty("time", Vect2f(0.0, 0.0));
			m_ControlPointColors.push_back(aux);
		}
	}
}

CParticleSystemType::~CParticleSystemType(void)
{
	Destroy();
}

void CParticleSystemType::Destroy()
{
}

void CParticleSystemType::Save(FILE* _File)
{
	fprintf_s(_File, "\t<particle name=\"%s\" material=\"%s\" frames=\"%i\" time=\"%f\" loop=\"%s\" emit_absolute=\"%s\" start_dir_angle=\"%f\" start_acc_angle=\"%f\""
		" size=\"%f %f\" emit_rate=\"%f %f\" awake_time=\"%f %f\" sleep_time=\"%f %f\" life=\"%f %f\" start_angle=\"%f %f\" start_angular_speed=\"%f %f\""
		" angular_acc=\"%f %f\" start_speed_1=\"%f %f %f\" start_speed_2=\"%f %f %f\" start_acc_1=\"%f %f %f\" start_acc_2=\"%f %f %f\" color_1=\"%f %f %f %f\" color_2=\"%f %f %f %f\">\n",
		m_Name.c_str(), GetMaterial()->GetName().c_str(), m_NumFrames, m_TimePerFrame, m_LoopFrames ? "true" : "false", m_EmitAbsolute ? "true" : "false", m_StartingDirectionAngle, 
		m_StartingAccelerationAngle, m_Size[0], m_Size[1], m_EmitRate[0], m_EmitRate[1], m_AwakeTime[0], m_AwakeTime[1], m_SleepTime[0], m_SleepTime[1], m_Life[0], m_Life[1], m_StartingAngle[0], 
		m_StartingAngle[1], m_StartingAngularSpeed[0], m_StartingAngularSpeed[1], m_AngularAcceleration[0], m_AngularAcceleration[1], m_StartingSpeed1[0], m_StartingSpeed1[1], 
		m_StartingSpeed1[2], m_StartingSpeed2[0], m_StartingSpeed2[1], m_StartingSpeed2[2], m_StartingAcceleration1[0], m_StartingAcceleration1[1], m_StartingAcceleration1[2],
		m_StartingAcceleration2[0], m_StartingAcceleration2[1], m_StartingAcceleration2[2], m_Color1.GetRed(), m_Color1.GetGreen(), m_Color1.GetBlue(), m_Color1.GetAlpha(),
		m_Color2.GetRed(), m_Color2.GetGreen(), m_Color2.GetBlue(), m_Color2.GetAlpha());
	for (size_t i = 0; i < m_ControlPointColors.size(); i++)
	{
		Vect2f auxTime = m_ControlPointColors[i].m_Time;
		CColor auxColor1 = m_ControlPointColors[i].m_Color1;
		CColor auxColor2 = m_ControlPointColors[i].m_Color2;
		fprintf_s(_File, "\t\t<control_point type=\"color\" time=\"%f %f\" color1=\"%f %f %f %f\" color2=\"%f %f %f %f\"/>\n",
			auxTime[0], auxTime[1], auxColor1.GetRed(), auxColor1.GetGreen(), auxColor1.GetBlue(), auxColor1.GetAlpha(), 
			auxColor2.GetRed(), auxColor2.GetGreen(), auxColor2.GetBlue(), auxColor2.GetAlpha());
	}

	for (size_t i = 0; i < m_ControlPointSizes.size(); i++)
	{
		Vect2f auxTime = m_ControlPointSizes[i].m_Time;
		Vect2f auxSize = m_ControlPointSizes[i].m_Size;
		fprintf_s(_File, "\t\t<control_point type=\"size\" time=\"%f %f\" size=\"%f %f\"/>\n",
			auxTime[0], auxTime[1], auxSize[0], auxSize[1]);
	}

	fprintf_s(_File, "\t</particle>\n");
}