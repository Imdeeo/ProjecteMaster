#include "Camera\CameraControllerManager.h"
#include "Camera\SphericalCameraController.h"
#include "Camera\FPSCameraController.h"
#include "Camera\CameraKeyController.h"
#include "Camera\CameraController.h"
#include "Camera\3PersonCameraController.h"
#include "XML\XMLTreeNode.h"

#include "Engine\UABEngine.h"

CCameraControllerManager::CCameraControllerManager():
	m_CurrentCamera(),
	m_CurrentCameraControl(0)
{

}

CCameraControllerManager::~CCameraControllerManager()
{
	Destroy();
}

void CCameraControllerManager::ChooseMainCamera(std::string _CurrentCamera)
{
	m_MainCamera = GetResource(_CurrentCamera);
}

void CCameraControllerManager::ChooseDebugCamera(std::string _CurrentCamera)
{
	GetResource(_CurrentCamera)->SetCamera(&m_CurrentCamera);
	UABEngine.GetRenderManager()->SetCurrentCamera(m_CurrentCamera);
	m_DebugCamera = GetResource(_CurrentCamera);
}


bool CCameraControllerManager::Load(const std::string &FileName)
{
	m_Filename = FileName;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["cameras"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("camera"))
				{
					CCamera::TCameraType l_Type = CCamera::GetCameraTypeByName(l_Element.GetPszProperty("type"));
					switch(l_Type)
					{
					case CCamera::CAMERA_TYPE_SPHERICAL:
						AddResource(l_Element.GetPszProperty("name"), new CSphericalCameraController());
						break;
					case CCamera::CAMERA_TYPE_FPS:
						AddResource(l_Element.GetPszProperty("name"), new CFPSCameraController());
						break;
					case CCamera::CAMERA_TYPE_3PS:
						AddResource(l_Element.GetPszProperty("name"), new C3PersonCameraController(l_Element));
						break;
					case CCamera::CAMERA_TYPE_CYCLE:
						AddResource(l_Element.GetPszProperty("name"), new CCameraKeyController(l_Element));
						break;
					case CCamera::CAMERA_TYPE_REVERSE:
						AddResource(l_Element.GetPszProperty("name"), new CCameraKeyController(l_Element));
						break;
					default:
						return false;
					}
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

bool CCameraControllerManager::Reload()
{
	Destroy();
	bool l_loadResult = Load(m_Filename);
	std::string l_CameraControllerStr;
	if (UABEngine.GetLevelLoaded() == "1")
	{
		l_CameraControllerStr = "Camera001";
	}
	else
	{
		l_CameraControllerStr = "FPSCamera";
	}
	UABEngine.GetCameraControllerManager()->ChooseMainCamera(l_CameraControllerStr);
	UABEngine.GetCameraControllerManager()->ChooseDebugCamera("SphericalCamera");
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