#include "Application.h"

#include "Math\Matrix44.h"
#include "Math\Vector4.h"

#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "InputManager\InputManager.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "DebugHelper.h"
#include "Engine\UABEngine.h"

static void __stdcall SwitchCameraCallback(void* _app)
{
	((CApplication*)_app)->SwitchCamera();
}

CApplication::CApplication( CContextManager *_ContextManager)
	: m_BackgroundColor(.2f, .1f, .4f)
	, m_CurrentCamera_control(0)
	, m_CurrentCamera_vision(0)
	,m_RenderCameraCube(false)
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::CApplication");

	UABEngine.GetRenderManager()->SetContextManager(_ContextManager);

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
	/*{
		CDebugHelper::SDebugVariable var = {};
		var.name = "cube";
		var.type = CDebugHelper::POSITION_ORIENTATION;
		var.mode = CDebugHelper::READ_WRITE;
		var.pPositionOrientation = m_Cube.GetPtrTransform();

		bar.variables.push_back(var);
	}*/
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
	UABEngine.Destroy();
}


void CApplication::SwitchCamera()
{
	m_CurrentCamera_vision++;
	m_CurrentCamera_vision = m_CurrentCamera_vision % 2;
	m_CurrentCamera_control = m_CurrentCamera_vision;
}

void CApplication::Update(float _ElapsedTime)
{	
	if(CInputManager::GetInputManager()->IsActionActive("RELOAD"))
	{
		CInputManager::GetInputManager()->reload();
		
		UABEngine.GetTextureManager()->Reload();

		UABEngine.GetEffectManager()->Reload();
		UABEngine.GetMaterialManager()->Reload();
		UABEngine.GetStaticMeshManager()->Reload();
		UABEngine.GetAnimatedModelsManager()->Reload();
		UABEngine.GetRenderableObjectsManager()->Reload();
	}
	if(CInputManager::GetInputManager()->IsActionActive("CHANGE_CAMERA_BOTH"))
	{

		SwitchCamera();
	}
	if(CInputManager::GetInputManager()->IsActionActive("CHANGE_CAMERA_VISION"))
	{

		m_CurrentCamera_vision++;
		m_CurrentCamera_vision = m_CurrentCamera_vision % 2;
	}
	if(CInputManager::GetInputManager()->IsActionActive("CHANGE_CAMERA_CONTROL"))
	{

		m_CurrentCamera_control++;
		m_CurrentCamera_control = m_CurrentCamera_control % 2;
	}
	if(CInputManager::GetInputManager()->IsActionActive("RENDER_CAMERA"))
	{
		m_RenderCameraCube = !m_RenderCameraCube;
	}

	if(CInputManager::GetInputManager()->IsActionActive("MONSTER_IDLE22"))
	{
		((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			ClearCycle(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			GetActualAnimation(),0.1f);
		((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->BlendCycle(0,1.f,0.1f);
	}
	if(CInputManager::GetInputManager()->IsActionActive("MONSTER_RUN"))
	{
		((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			ClearCycle(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			GetActualAnimation(),0.1f);
		((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->BlendCycle(1,1.f,0.1f);
	}
	if(CInputManager::GetInputManager()->IsActionActive("MONSTER_HIT"))
	{
		((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			ClearCycle(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			GetActualAnimation(),0.1f);
		((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->ExecuteAction(2,0.1f,0.1f);
	}
	switch (m_CurrentCamera_control)
	{
	case 0:
		if (CInputManager::GetInputManager()->IsActionActive("MOVE_CAMERA"))
		{
			CSphericalCameraController* l_SphericalCameraController = (CSphericalCameraController*)UABEngine.GetCameraManager()->GetResource("SphericalCamera");
			Vect3f cameraMovement(0, 0, 0);

			cameraMovement.x += CInputManager::GetInputManager()->GetAxis("X_AXIS") * _ElapsedTime * 0.5f;
			cameraMovement.y += CInputManager::GetInputManager()->GetAxis("Y_AXIS") * _ElapsedTime * 0.5f;

			l_SphericalCameraController->Update(cameraMovement);
		}
		break;
	case 1:
		{
			CFPSCameraController* l_FPSCameraController = (CFPSCameraController*)UABEngine.GetCameraManager()->GetResource("FPSCamera");
			l_FPSCameraController->AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * _ElapsedTime * 0.05f);
			l_FPSCameraController->AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS") * _ElapsedTime * -0.05f);

			l_FPSCameraController->Move(CInputManager::GetInputManager()->GetAxis("STRAFE"), CInputManager::GetInputManager()->GetAxis("MOVE_FWD"), false, _ElapsedTime);
		}
		break;
	}

	{
		//std::string l_CamaraController = "FPSCamera";
		std::string l_CamaraController = "Camera001";
		CCamera camera;
		CCameraController* l_CameraController = (CCameraController*)UABEngine.GetCameraManager()->GetResource(l_CamaraController);
		l_CameraController->Update(_ElapsedTime);
		l_CameraController->SetCamera(&camera);

		UABEngine.GetRenderManager()->SetCurrentCamera(camera);


		CSphericalCameraController* l_SphericalCameraController = (CSphericalCameraController*)UABEngine.GetCameraManager()->GetResource("SphericalCamera");
		l_SphericalCameraController->SetCamera(&camera);
		UABEngine.GetRenderManager()->SetDebugCamera(camera);

		UABEngine.GetRenderManager()->SetUseDebugCamera(m_CurrentCamera_vision == 0);
	}

	UABEngine.GetRenderableObjectsManager()->Update(_ElapsedTime);

}

void CApplication::Render()
{
	UABEngine.GetRenderManager()->Render();
}


void CApplication::Init()
{
	UABEngine.Init();
}
