#pragma once

#include "Camera\SphericalCameraController.h"
#include "Camera\FPSCameraController.h"
#include "HelperTypes.h"
#include "RenderManager\RenderManager.h"
#include "Materials\MaterialManager.h"

class CDebugRender;
class CContextManager;

class CApplication
{
public:
	CApplication(CContextManager *_ContextManager);
	~CApplication();

	void SwitchCamera();

	void Update(float _ElapsedTime);
	void Render();

	void Init();

private:

	CSphericalCameraController m_SphericalCamera;
	CFPSCameraController m_FPSCamera;

	int m_CurrentCamera_vision;
	int m_CurrentCamera_control;

	bool m_RenderCameraCube;

	//CDebugRender *m_DebugRender;

	CColor m_BackgroundColor;

};

