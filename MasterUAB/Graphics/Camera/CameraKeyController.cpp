#include "Camera\CameraKeyController.h"
#include "Camera\CameraKey.h"

#include "Engine\UABEngine.h"

CCameraKeyController::CCameraKeyController(CXMLTreeNode &XMLTreeNode)
	:m_CurrentTime(0),
	m_CurrentKey(0),
	m_NextKey(1),
	m_Cycle(false),
	m_Reverse(false)
{
	m_CurrentTime = 0;
	m_TotalTime = 30.0f/(*XMLTreeNode.GetPszProperty("total_time"));
	std::string l_Filename;
	l_Filename = *XMLTreeNode.GetPszProperty("filename");
	std::string l_Type;
	l_Type = *XMLTreeNode.GetPszProperty("type");
	if(l_Type == "cycle"){ m_Cycle = true; }
	if(l_Type == "reverse"){ m_Reverse = true; }
	LoadXML(l_Filename);
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
					l_Time = 30.0f/(std::stof(l_Element.GetPszProperty("key")));

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
	for(size_t i = 0; i < m_Keys.size(); i++){
		if (m_CurrentTime >= m_Keys[i]->m_Time){ m_CurrentKey = i; }
	}
	m_NextKey = m_CurrentKey+1;
	if(m_NextKey >= m_Keys.size()){ m_NextKey = 0; }
}

void CCameraKeyController::Update(float ElapsedTime)
{
	m_CurrentTime += ElapsedTime;
	/*
	Determinar el tiempo que ha pasado, blabla, para poder
	obtener los parámetros actuales de la cámara, después,
	hacer todos los sets igual que en SetCamera, pero usando
	m_Camera, que es una referencia a la cámara del juego.
	*/
}

void CCameraKeyController::SetCamera(CCamera *Camera) const
{
	m_Camera = Camera;

	m_Camera->SetPosition(m_Position);
	m_Camera->SetLookAt(m_Position);
	m_Camera->SetUp(GetUp());
	m_Camera->SetFOV(1.047f);
	m_Camera->SetAspectRatio(UABEngine.GetRenderManager()->GetContextManager()->GetAspectRatio());
	m_Camera->SetZNear(0.1f);
	m_Camera->SetZFar(100.f);

	m_Camera->SetMatrixs();
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