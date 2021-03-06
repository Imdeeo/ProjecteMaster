#ifndef APPLICATION_H
#define APPLICATION_H

#include "Camera\SphericalCameraController.h"
#include "Camera\FPSCameraController.h"
#include "HelperTypes.h"
#include "RenderManager\RenderManager.h"
#include "Materials\MaterialManager.h"

class CRenderableObject;
class CDebugRender;
class CContextManager;

class CApplication
{
public:
	CApplication(CContextManager *_ContextManager);
	~CApplication();

	void Update(float _ElapsedTime);
	void Render();

	void Init();
	static void ActualizarEnemigo(CRenderableObject* owner, float _ElapsedTime);
private:
	std::string m_DebugCameraName;
	std::string m_MainCameraName;

	CSphericalCameraController* m_SphericalCamera;
	CFPSCameraController* m_FPSCamera;

	bool m_RenderCameraCube;

	//CDebugRender *m_DebugRender;

	CColor m_BackgroundColor;

	float m_Timer;
};

#endif