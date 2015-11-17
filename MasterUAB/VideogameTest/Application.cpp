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
		var.pPositionOrientation = m_Cube.GetPtrTransform();

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
		m_MaterialManager.ClearMaterials();
		m_MaterialManager.AddMaterials("Data\\materials.xml");
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

	{
		CCamera camera;
		m_FPSCamera.SetCamera(&camera);
		camera.SetFOV(1.047f);
		camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
		camera.SetZNear(0.1f);
		camera.SetZFar(100.f);
		camera.SetMatrixs();
		m_RenderManager.SetCurrentCamera(camera);

		m_SphericalCamera.SetCamera(&camera);
		camera.SetFOV(1.047f);
		camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
		camera.SetZNear(0.1f);
		camera.SetZFar(100.f);
		camera.SetMatrixs();
		m_RenderManager.SetDebugCamera(camera);

		m_RenderManager.SetUseDebugCamera(m_CurrentCamera == 0);
	}

}

void CApplication::Render()
{
	m_ContextManager->BeginRender(m_BackgroundColor);

	// añadir todos los objetos que se quiere pintar
	m_RenderManager.AddRenderableObjectToRenderList(&m_Cube);
	m_RenderManager.AddRenderableObjectToRenderList(&m_Triangle1);
	m_RenderManager.AddRenderableObjectToRenderList(&m_Triangle2);

	m_RenderManager.Render(m_ContextManager, &m_MaterialManager);


	Mat44f world;

	world.SetIdentity();
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetAxis());

	/*world.SetIdentity();
	world.SetFromPos(10, 0, 0);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetClassicBlendTriangle(), CContextManager::RS_SOLID_NO_CULL, CContextManager::DSS_DEPTH_ON, CContextManager::BLEND_CLASSIC);

	world.SetIdentity();
	world.SetFromPos(0, 0, -10);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetPremultBlendTriangle(), CContextManager::RS_SOLID_NO_CULL, CContextManager::DSS_DEPTH_ON, CContextManager::BLEND_PREMULT);
	*/

	CDebugHelper::GetDebugHelper()->Render();

	m_ContextManager->EndRender();
}


void CApplication::Init()
{
	m_MaterialManager.AddMaterials("Data\\materials.xml");

	m_Cube.AddSubmesh(m_DebugRender->GetSimpleCube(), "solid material", m_DebugRender->GetSimpleCubeBSRadi(), m_DebugRender->GetSimpleCubeBBMin(), m_DebugRender->GetSimpleCubeBBMax());
	m_Triangle1.AddSubmesh(m_DebugRender->GetClassicBlendTriangle(),"classic blend",m_DebugRender->GetClassicBlendTriangleBSRadi(),m_DebugRender->GetClassicBlendTriangleBBMin(),m_DebugRender->GetClassicBlendTriangleBBMax());
	m_Triangle2.AddSubmesh(m_DebugRender->GetClassicBlendTriangle(),"premult blend",m_DebugRender->GetClassicBlendTriangleBSRadi(),m_DebugRender->GetClassicBlendTriangleBBMin(),m_DebugRender->GetClassicBlendTriangleBBMax());

	m_Triangle1.SetPosition(Vect3f(10,0,0));
	m_Triangle2.SetPosition(Vect3f(0,0,-10));
}
