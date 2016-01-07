#ifndef CAMERA_CONTROLLER_MANAGER_H
#define CAMERA_CONTROLLER_MANAGER_H

#include "TemplatedMapManager.h"
#include "Camera\Camera.h"
#include <string>

class CCameraControllerManager  :	public CTemplatedMapManager<CCameraController>
{
private:
	std::string m_Filename;
public:
	CCameraControllerManager(void){}
	virtual ~CCameraControllerManager(void){}

	void SetCurrentCamera(const CCamera& _CurrentCamera);
	bool Load(const std::string &FileName);
	bool Reload();

private:
	CCamera m_CurrentCamera;

};

#endif //CAMERA_CONTROLLER_MANAGER_H
