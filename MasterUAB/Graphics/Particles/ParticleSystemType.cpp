#include "ParticleSystemType.h"
#include "Engine\UABEngine.h"

#include "LevelManager\Level.h"

CParticleSystemType::CParticleSystemType(tinyxml2::XMLElement* TreeNode, CLevel *_Level) : CNamed(TreeNode), CLevelInfo(_Level)
{
	m_Level = _Level->GetName();
	m_Material = _Level->GetMaterialManager()->GetResource(TreeNode->GetPszProperty("material"));
	m_NumFrames = TreeNode->GetIntProperty("frames", 1);
	m_TimePerFrame = TreeNode->GetFloatProperty("time", 1.0f);
	m_LoopFrames = TreeNode->GetBoolProperty("loop", false);
	m_EmitAbsolute = TreeNode->GetBoolProperty("emit_absolute", false);
	m_StartingDirectionAngle = TreeNode->GetFloatProperty("start_dir_angle", 1.0);
	m_StartingAccelerationAngle = TreeNode->GetFloatProperty("start_acc_angle", 1.0);
	m_EmitRate = TreeNode->GetVect2fProperty("emit_rate", Vect2f(0.0, 0.0));
	m_AwakeTime = TreeNode->GetVect2fProperty("awake_time", Vect2f(0.0, 0.0));
	m_SleepTime = TreeNode->GetVect2fProperty("sleep_time", Vect2f(0.0, 0.0));
	m_Life = TreeNode->GetVect2fProperty("life", Vect2f(0.0, 0.0));
	m_StartingAngle = TreeNode->GetVect2fProperty("start_angle", Vect2f(0.0, 0.0));
	m_StartingAngularSpeed = TreeNode->GetVect2fProperty("start_angular_speed", Vect2f(0.0, 0.0));
	m_AngularAcceleration = TreeNode->GetVect2fProperty("angular_acc", Vect2f(0.0, 0.0));
	
	tinyxml2::XMLElement* l_Element = TreeNode->FirstChildElement();

	while (l_Element != NULL)
	{
		if (l_Element->GetPszProperty("type") == std::string("size"))
		{
			ControlPointSize aux;
			aux.m_Size = l_Element->GetVect2fProperty("size", Vect2f(0.0, 0.0));
			aux.m_Time = l_Element->GetVect2fProperty("time", Vect2f(0.0, 0.0));
			m_ControlPointSizes.push_back(aux);
		}
		else if (l_Element->GetPszProperty("type") == std::string("color"))
		{
			ControlPointColor aux;
			aux.m_Color1 = CColor(l_Element->GetVect4fProperty("color1", Vect4f(0.0, 0.0, 0.0, 1.0)));
			aux.m_Color2 = CColor(l_Element->GetVect4fProperty("color2", Vect4f(0.0, 0.0, 0.0, 1.0)));
			aux.m_Time = l_Element->GetVect2fProperty("time", Vect2f(0.0, 0.0));
			m_ControlPointColors.push_back(aux);
		}
		else if (l_Element->GetPszProperty("type") == std::string("speed"))
		{
			ControlPointSpeed aux;
			aux.m_Speed1 = l_Element->GetVect3fProperty("speed_1", Vect3f(0.0, 0.0, 0.0));
			aux.m_Speed2 = l_Element->GetVect3fProperty("speed_2", Vect3f(0.0, 0.0, 0.0));
			aux.m_Time = l_Element->GetVect2fProperty("time", Vect2f(0.0, 0.0));
			m_ControlPointSpeeds.push_back(aux);
		}
		else if (l_Element->GetPszProperty("type") == std::string("acceleration"))
		{
			ControlPointAcceleration aux;
			aux.m_Acceleration1 = l_Element->GetVect3fProperty("acceleration_1", Vect3f(0.0, 0.0, 0.0));
			aux.m_Acceleration2 = l_Element->GetVect3fProperty("acceleration_2", Vect3f(0.0, 0.0, 0.0));
			aux.m_Time = l_Element->GetVect2fProperty("time", Vect2f(0.0, 0.0));
			m_ControlPointAccelerations.push_back(aux);
		}
		l_Element = l_Element->NextSiblingElement();
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
		" emit_rate=\"%f %f\" awake_time=\"%f %f\" sleep_time=\"%f %f\" life=\"%f %f\" start_angle=\"%f %f\" start_angular_speed=\"%f %f\" angular_acc=\"%f %f\">\n",
		m_Name.c_str(), GetMaterial()->GetName().c_str(), m_NumFrames, m_TimePerFrame, m_LoopFrames ? "true" : "false", m_EmitAbsolute ? "true" : "false", m_StartingDirectionAngle, 
		m_StartingAccelerationAngle, m_EmitRate[0], m_EmitRate[1], m_AwakeTime[0], m_AwakeTime[1], m_SleepTime[0], m_SleepTime[1], m_Life[0], m_Life[1], m_StartingAngle[0], 
		m_StartingAngle[1], m_StartingAngularSpeed[0], m_StartingAngularSpeed[1], m_AngularAcceleration[0], m_AngularAcceleration[1]);
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

	for (size_t i = 0; i < m_ControlPointSpeeds.size(); i++)
	{
		Vect2f auxTime = m_ControlPointSpeeds[i].m_Time;
		Vect3f auxSpeed1 = m_ControlPointSpeeds[i].m_Speed1;
		Vect3f auxSpeed2 = m_ControlPointSpeeds[i].m_Speed2;
		fprintf_s(_File, "\t\t<control_point type=\"speed\" time=\"%f %f\" speed_1=\"%f %f %f\" speed_2=\"%f %f %f\"/>\n",
			auxTime[0], auxTime[1], auxSpeed1[0], auxSpeed1[1], auxSpeed1[2], auxSpeed2[0], auxSpeed2[1], auxSpeed2[2]);
	}

	for (size_t i = 0; i < m_ControlPointAccelerations.size(); i++)
	{
		Vect2f auxTime = m_ControlPointAccelerations[i].m_Time;
		Vect3f auxAcc1 = m_ControlPointAccelerations[i].m_Acceleration1;
		Vect3f auxAcc2 = m_ControlPointAccelerations[i].m_Acceleration2;
		fprintf_s(_File, "\t\t<control_point type=\"acceleration\" time=\"%f %f\" acceleration_1=\"%f %f %f\" acceleration_2=\"%f %f %f\"/>\n",
			auxTime[0], auxTime[1], auxAcc1[0], auxAcc1[1], auxAcc1[2], auxAcc2[0], auxAcc2[1], auxAcc2[2]);
	}

	fprintf_s(_File, "\t</particle>\n");
}