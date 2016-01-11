#ifndef CAMERA_CONTROLLER_MANAGER_H
#define CAMERA_CONTROLLER_MANAGER_H

#include "TemplatedMapManager.h"
#include "CameraController.h"
#include "Camera\Camera.h"
#include <string>

class CCamera;

class CCameraControllerManager : public CTemplatedMapManager<CCameraController>
{
private:
	std::string m_Filename;
	CCamera m_CurrentCamera;
public:
	CCameraControllerManager(void);
	virtual ~CCameraControllerManager(void);

	void ChooseCurrentCamera(std::string _CurrentCamera);
	bool Load(const std::string &FileName);
	bool Reload();
	

};

#endif //CAMERA_CONTROLLER_MANAGER_H
