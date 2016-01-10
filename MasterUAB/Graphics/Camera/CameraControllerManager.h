#ifndef CAMERA_CONTROLLER_MANAGER_H
#define CAMERA_CONTROLLER_MANAGER_H

#include "TemplatedMapManager.h"
#include "CameraController.h"
#include <string>

class CCamera;

class CCameraControllerManager : public CTemplatedMapManager<CCameraController>
{
private:
	std::string m_Filename;
public:
	CCameraControllerManager(void);
	virtual ~CCameraControllerManager(void);

	void ChooseCurrentCamera(std::string _CurrentCamera);
	bool Load(const std::string &FileName);
	bool Reload();
<<<<<<< HEAD
	
=======

private:
	CCamera m_CurrentCamera;
>>>>>>> develop

};

#endif //CAMERA_CONTROLLER_MANAGER_H
