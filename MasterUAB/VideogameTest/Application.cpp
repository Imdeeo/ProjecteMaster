#include "Application.h"

#include "Math\Matrix44.h"
#include "Math\Vector4.h"

#include "ContextManager.h"
#include "DebugRender.h"
#include "InputManager.h"
#include "DebugHelper.h"


static void __stdcall SwitchCameraCallback(void* _app)
{
	((CApplication*)_app)->SwitchCamera();
}

CApplication::CApplication(CDebugRender *_DebugRender, CContextManager *_ContextManager)
	: m_DebugRender(_DebugRender)
	, m_ContextManager(_ContextManager)
	, m_BackgroundColor(.2f, .1f, .4f)
	, m_CurrentCamera(0)
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::CApplication");

	CDebugHelper::SDebugBar bar;
	bar.name = "CApplication";

	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "background";
		var.type = CDebugHelper::COLOR;
		var.mode = CDebugHelper::READ_WRITE;
		var.pColor = &m_BackgroundColor;

		bar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "cube";
		var.type = CDebugHelper::POSITION_ORIENTATION;
		var.mode = CDebugHelper::READ_WRITE;
		var.pPositionOrientation = &m_CubeTransform;

		bar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "switch camera";
		var.type = CDebugHelper::BUTTON;
		var.callback = SwitchCameraCallback;
		var.data = this;

		bar.variables.push_back(var);
	}

	CDebugHelper::GetDebugHelper()->RegisterBar(bar);

}


CApplication::~CApplication()
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::~CApplication");
}


void CApplication::SwitchCamera()
{
	++m_CurrentCamera;
	if (m_CurrentCamera > 1)
	{
		m_CurrentCamera = 0;
	}
}

void CApplication::Update(float _ElapsedTime)
{	
	if(CInputManager::GetInputManager()->IsActionActive("RELOAD"))
	{
		CInputManager::GetInputManager()->reload();
	}
	switch (m_CurrentCamera)
	{
	case 0:
		if (CInputManager::GetInputManager()->IsActionActive("MOVE_CAMERA"))
		{
			Vect3f cameraMovement(0, 0, 0);

			cameraMovement.x += CInputManager::GetInputManager()->GetAxis("X_AXIS") * _ElapsedTime * 0.5f;
			cameraMovement.y += CInputManager::GetInputManager()->GetAxis("Y_AXIS") * _ElapsedTime * 0.5f;

			m_SphericalCamera.Update(cameraMovement);
		}
		break;
	case 1:
	{
		m_FPSCamera.AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * _ElapsedTime * 0.05f);
		m_FPSCamera.AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS") * _ElapsedTime * -0.05f);

		m_FPSCamera.Move(CInputManager::GetInputManager()->GetAxis("STRAFE"), CInputManager::GetInputManager()->GetAxis("MOVE_FWD"), false, _ElapsedTime);
	}
		break;
	}
}

void CApplication::Render()
{
	m_ContextManager->BeginRender(m_BackgroundColor);

	CCamera camera;
	camera.SetFOV(1.047f);
	camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
	camera.SetZNear(0.1f);
	camera.SetZFar(100.f);

	switch (m_CurrentCamera)
	{
	case 0:
		m_SphericalCamera.SetCamera(&camera);
		break;
	case 1:
		m_FPSCamera.SetCamera(&camera);
		break;
	default:
		m_CurrentCamera = 0;
		break;
	}

	camera.SetMatrixs();

	Mat44f world;

	world.SetFromPosAndAnglesYXZ(m_CubeTransform.Position, m_CubeTransform.Yaw, m_CubeTransform.Pitch, m_CubeTransform.Roll);

	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->SetCamera(camera);

	m_ContextManager->SetDebugSize(5);
	m_ContextManager->SetBaseColor(CColor(1, 1, 1, 1));

	m_ContextManager->Draw(m_DebugRender->GetSimpleCube(), CContextManager::RS_SOLID_BACK_CULL, CContextManager::DSS_TEST_WRITE, CContextManager::BLEND_CLASSIC);

	world.SetIdentity();
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetAxis(), CContextManager::RS_SOLID_BACK_CULL, CContextManager::DSS_TEST_WRITE, CContextManager::BLEND_CLASSIC);

	world.SetIdentity();
	world.SetFromPos(10, 0, 0);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetClassicBlendTriangle(), CContextManager::RS_SOLID_NO_CULL, CContextManager::DSS_TEST_WRITE, CContextManager::BLEND_CLASSIC);

	world.SetIdentity();
	world.SetFromPos(0, 0, -10);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetPremultBlendTriangle(), CContextManager::RS_SOLID_NO_CULL, CContextManager::DSS_TEST_WRITE, CContextManager::BLEND_PREMULT);


	CDebugHelper::GetDebugHelper()->Render();

	m_ContextManager->EndRender();
}
