#include "Camera\CameraControllerManager.h"
#include "Camera\SphericalCameraController.h"
#include "Camera\FPSCameraController.h"
#include "Camera\CameraKeyController.h"
#include "Camera\CameraController.h"
#include "XML\XMLTreeNode.h"

#include "Engine\UABEngine.h"

CCameraControllerManager::CCameraControllerManager():
	m_CurrentCamera()
{

}

CCameraControllerManager::~CCameraControllerManager()
{
	Destroy();
}

void CCameraControllerManager::ChooseCurrentCamera(std::string _CurrentCamera)
{
	GetResource(_CurrentCamera)->SetCamera(&m_CurrentCamera);
	UABEngine.GetRenderManager()->SetCurrentCamera(m_CurrentCamera);
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
	return Load(m_Filename);
}