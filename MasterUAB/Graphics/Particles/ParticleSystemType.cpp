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