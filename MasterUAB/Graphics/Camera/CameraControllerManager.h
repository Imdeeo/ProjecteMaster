#ifndef CAMERA_CONTROLLER_MANAGER_H
#define CAMERA_CONTROLLER_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Camera\CameraController.h"
#include "Camera\Camera.h"
#include <string>

class CCamera;

class CCameraControllerManager : public CTemplatedMapManager<CCameraController>
{
private:
	std::string m_Filename;
	CCamera m_CurrentCamera;
	CCameraController* m_MainCamera;
	CCameraController* m_DebugCamera;
public:
	CCameraControllerManager(void);
	virtual ~CCameraControllerManager(void);

	void ChooseMainCamera(std::string _CurrentCamera);
	void ChooseDebugCamera(std::string _CurrentCamera);
	bool Load(const std::string &FileName);
	bool Reload();
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

private:
	CCamera m_CurrentCamera;
>>>>>>> develop
=======
	CCameraController* GetMainCamera(){ return m_MainCamera; };
	CCameraController* GetDebugCamera(){ return m_DebugCamera; };
	void UpdateMainCamera(float _ElapsedTime);
	void UpdateDebugCamera(float _ElapsedTime);
	void Update(float _ElapsedTime);
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7

};

#endif //CAMERA_CONTROLLER_MANAGER_H
