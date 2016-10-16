#include "Camera\CameraControllerManager.h"
#include "Camera\SphericalCameraController.h"
#include "Camera\FPSCameraController.h"
#include "Camera\CameraKeyController.h"
#include "Camera\CameraController.h"
#include "Camera\3PersonCameraController.h"
#include "XML\tinyxml2.h"

#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"

#include "Engine\UABEngine.h"
#include "LevelManager\LevelManager.h"

CCameraControllerManager::CCameraControllerManager():
	m_CurrentCamera(),
	m_CurrentCameraControl(1)
{

}

CCameraControllerManager::~CCameraControllerManager()
{
	Destroy();
}

void CCameraControllerManager::ChooseMainCamera(std::string _CurrentCamera)
{
	m_MainCamera = GetResource(_CurrentCamera);
	m_MainCamera->SetCamera(&m_CurrentCamera);
	UABEngine.GetRenderManager()->SetCurrentCamera(m_CurrentCamera);
}

void CCameraControllerManager::ChooseDebugCamera(std::string _CurrentCamera)
{
	m_DebugCamera = GetResource(_CurrentCamera);
	m_DebugCamera->SetCamera(&m_CurrentCamera);
	UABEngine.GetRenderManager()->SetCurrentCamera(m_CurrentCamera);
}


bool CCameraControllerManager::Load(const std::string &FileName, const std::string &_LevelId)
{
	m_Filename = FileName;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(FileName.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("cameras");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux != NULL)
			{
				if (l_ElementAux->Name() == std::string("camera"))
				{
					CCamera::TCameraType l_Type = CCamera::GetCameraTypeByName(l_ElementAux->GetPszProperty("type"));
					switch(l_Type)
					{
					case CCamera::CAMERA_TYPE_SPHERICAL:
						AddResource(l_ElementAux->GetPszProperty("name"), new CSphericalCameraController(l_ElementAux,_LevelId),_LevelId);
						break;
					case CCamera::CAMERA_TYPE_FPS:
						AddResource(l_ElementAux->GetPszProperty("name"), new CFPSCameraController(l_ElementAux,_LevelId),_LevelId);
						break;
					case CCamera::CAMERA_TYPE_3PS:
						AddResource(l_ElementAux->GetPszProperty("name"), new C3PersonCameraController(l_ElementAux,_LevelId),_LevelId);
						break;
					case CCamera::CAMERA_TYPE_KEY:
						AddResource(l_ElementAux->GetPszProperty("name"), new CCameraKeyController(l_ElementAux,_LevelId),_LevelId);
						break;				
					default:
						return false;
					}
				}
				l_ElementAux = l_ElementAux->NextSiblingElement();
			}
		}
	}
	else
	{
		return false;
	}
	Init();
	return true;
}

void CCameraControllerManager::Init()
{
	UABEngine.GetCameraControllerManager()->ChooseMainCamera("MainCamera");
	UABEngine.GetCameraControllerManager()->ChooseDebugCamera("DebugCamera");
}

bool CCameraControllerManager::Reload()
{
	Destroy();
	bool l_loadResult = Load(m_Filename,"");
	return l_loadResult;
}

void CCameraControllerManager::UpdateMainCamera(float _ElapsedTime)
{
	CCamera l_Camera;
	CCameraController* l_CameraController = GetMainCamera();
	l_CameraController->Update(_ElapsedTime);
	l_CameraController->SetCamera(&l_Camera);
	l_Camera.SetAspectRatio(UABEngine.GetRenderManager()->GetContextManager()->GetAspectRatio());
	UABEngine.GetRenderManager()->SetCurrentCamera(l_Camera);
}

void CCameraControllerManager::UpdateDebugCamera(float _ElapsedTime)
{
	CCamera l_Camera;
	CCameraController* l_CameraController = GetDebugCamera();
	l_CameraController->Update(_ElapsedTime);
	l_CameraController->SetCamera(&l_Camera);
	l_Camera.SetAspectRatio(UABEngine.GetRenderManager()->GetContextManager()->GetAspectRatio());
	UABEngine.GetRenderManager()->SetDebugCamera(l_Camera);
}

void CCameraControllerManager::Update(float _ElapsedTime)
{
	switch (m_CurrentCameraControl)
	{
	case 0:
		UpdateDebugCamera(_ElapsedTime);
		break;
	case 1:
		UpdateMainCamera(_ElapsedTime); 
		break;
	}
}