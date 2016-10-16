#ifndef CAMERA_CONTROLLER_MANAGER_H
#define CAMERA_CONTROLLER_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Camera\CameraController.h"
#include "Camera\Camera.h"
#include <string>

class CCamera;

class CCameraControllerManager : public CTemplatedLevelMapManager<CCameraController>
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
	bool Load(const std::string &FileName, const std::string &_LevelId);
	bool Reload();
	void Update(float _ElapsedTime);

	CCameraController* 
		GetMainCamera(){ return m_MainCamera; };
	CCameraController* GetDebugCamera(){ return m_DebugCamera; };
	UAB_BUILD_GET_SET(int, CurrentCameraControl)
private:
	void UpdateMainCamera(float _ElapsedTime);
	void UpdateDebugCamera(float _ElapsedTime);
	void Init();
};

#endif //CAMERA_CONTROLLER_MANAGER_H
