#pragma once

#include "SphericalCameraController.h"
#include "FPSCameraController.h"
#include "HelperTypes.h"

class CDebugRender;
class CContextManager;

class CApplication
{
public:
	CApplication(CDebugRender *_DebugRender, CContextManager *_ContextManager);
	~CApplication();

	void SwitchCamera();

	void Update(float _ElapsedTime);
	void Render();

private:

	CSphericalCameraController m_SphericalCamera;
	CFPSCameraController m_FPSCamera;

	int m_CurrentCamera;

	CDebugRender *m_DebugRender;
	CContextManager *m_ContextManager;

	CColor m_BackgroundColor;
	SPositionOrientation m_CubeTransform;
};

