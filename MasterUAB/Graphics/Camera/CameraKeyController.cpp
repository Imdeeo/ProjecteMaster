#include "CameraKeyController.h"
#include "CameraKey.h"

CCameraKeyController::CCameraKeyController(CXMLTreeNode &XMLTreeNode)
{
	m_CurrentTime = 0;
	m_TotalTime = *XMLTreeNode.GetPszProperty("total_time");
	std::string l_Filename;
	l_Filename = *XMLTreeNode.GetPszProperty("filename");
	LoadXML(l_Filename);
	m_Cycle = *XMLTreeNode.GetPszProperty("cycle");
	m_Reverse = *XMLTreeNode.GetPszProperty("reverse");
}

CCameraKeyController::~CCameraKeyController()
{
}

bool CCameraKeyController::LoadXML(const std::string &FileName)
{
	float l_Time;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["camera_key_controller"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("key"))
				{
					l_Time = std::stof(l_Element.GetPszProperty("time"));

					CCameraInfo *l_CameraInfo = new CCameraInfo(l_Element);
					CCameraKey *l_CameraKey = new CCameraKey(*l_CameraInfo, l_Time);

					m_Keys.push_back(l_CameraKey);
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

void CCameraKeyController::GetCurrentKey()
{

}

void CCameraKeyController::Update(float ElapsedTime)
{
}

void CCameraKeyController::SetCurrentTime(float CurrentTime)
{
	m_CurrentTime = CurrentTime;
}

void CCameraKeyController::ResetTime()
{
	m_CurrentTime = 0;
}

float CCameraKeyController::GetTotalTime()
{
	return m_TotalTime;
}

bool CCameraKeyController::IsCycle() const
{
	return m_Cycle;
}

void CCameraKeyController::SetCycle(bool Cycle)
{
	m_Cycle = Cycle;
}

bool CCameraKeyController::IsReverse() const
{
	return m_Reverse;
}

void CCameraKeyController::SetReverse(bool Reverse)
{
	m_Reverse = Reverse;
}