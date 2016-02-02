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
static void __stdcall ReloadCamerasManager(void* _app)
{
	UABEngine.GetCameraControllerManager()->Reload();
}
static void __stdcall ReloadRenderableObjectsManager(void* _app)
{
	UABEngine.GetLayerManager()->Reload();
}
static void __stdcall ReloadLights(void* _app)
{
	UABEngine.GetLightManager()->Reload();
}
static void __stdcall ReloadMaterials(void* _app)
{
	UABEngine.GetMaterialManager()->Reload();
	UABEngine.GetStaticMeshManager()->Reload();
}
static void __stdcall ReloadTextures(void* _app)
{
	UABEngine.GetTextureManager()->Reload();
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


	CDebugHelper::SDebugBar bar2;
	bar2.name = "Reload";
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "RenderableObjectsManager";
		var.type = CDebugHelper::BUTTON;
		var.callback = ReloadRenderableObjectsManager;
		var.data = this;

		bar2.variables.push_back(var);
	}

	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "CameraManager";
		var.type = CDebugHelper::BUTTON;
		var.callback = ReloadCamerasManager;
		var.data = this;

		bar2.variables.push_back(var);
	}

	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Lights";
		var.type = CDebugHelper::BUTTON;
		var.callback = ReloadLights;
		var.data = this;

		bar2.variables.push_back(var);
	}

	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Materials Manager";
		var.type = CDebugHelper::BUTTON;
		var.callback = ReloadMaterials;
		var.data = this;

		bar2.variables.push_back(var);
	}

		{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Textures Manager";
		var.type = CDebugHelper::BUTTON;
		var.callback = ReloadTextures;
		var.data = this;

		bar2.variables.push_back(var);
	}

	CDebugHelper::GetDebugHelper()->RegisterBar(bar);
	CDebugHelper::GetDebugHelper()->RegisterBar(bar2);

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
		UABEngine.GetLightManager()->Reload();
		UABEngine.GetEffectManager()->Reload();
		UABEngine.GetMaterialManager()->Reload();
		UABEngine.GetStaticMeshManager()->Reload();
		UABEngine.GetAnimatedModelsManager()->Reload();
		UABEngine.GetLayerManager()->Reload();
	}
	if (CInputManager::GetInputManager()->IsActionActive("SWITCH_RENDER_LIGHTS"))
	{
		UABEngine.GetLightManager()->SwitchRenderLights();
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

	/*if(CInputManager::GetInputManager()->IsActionActive("MONSTER_IDLE"))
	{
		if(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")!=nullptr)
		{
			if(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->IsActionAnimationActive(2))
			{
				((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
					RemoveAction(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
					GetActualActionAnimation());
			}
			((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			ClearCycle(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			GetActualCycleAnimation(),0.1f);
			((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->BlendCycle(0,1.f,0.1f);
		}
	}
	if(CInputManager::GetInputManager()->IsActionActive("MONSTER_RUN"))
	{
		if(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")!=nullptr)
		{
			if(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->IsActionAnimationActive(2))
			{
				((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
					RemoveAction(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
				GetActualActionAnimation());
			}
			((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			ClearCycle(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			GetActualCycleAnimation(),0.1f);
			((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->BlendCycle(1,1.f,0.1f);
		}
	}
	if(CInputManager::GetInputManager()->IsActionActive("MONSTER_HIT"))
	{
		if(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")!=nullptr)
		{
			if(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->IsActionAnimationActive(2))
			{
				((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
					RemoveAction(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
				GetActualActionAnimation());
			}
			else
			{*/
				/*((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
				ClearCycle(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
				GetActualAnimation(),0.1f);*/
			/*}
		((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->ExecuteAction(2,0.1f,0.1f);
		}
	}*/

	switch (m_CurrentCamera_control)
	{
	case 0:
		if (CInputManager::GetInputManager()->IsActionActive("MOVE_CAMERA"))
		{
			CSphericalCameraController* l_SphericalCameraController = (CSphericalCameraController*)UABEngine.GetCameraControllerManager()->GetResource("SphericalCamera");
			Vect3f cameraMovement(0, 0, 0);

			cameraMovement.x += CInputManager::GetInputManager()->GetAxis("X_AXIS") * _ElapsedTime * 0.5f;
			cameraMovement.y += CInputManager::GetInputManager()->GetAxis("Y_AXIS") * _ElapsedTime * 0.5f;

			l_SphericalCameraController->Move(cameraMovement);
		}
		break;
	case 1:
		{
			CFPSCameraController* l_FPSCameraController = (CFPSCameraController*)UABEngine.GetCameraControllerManager()->GetResource(m_MainCameraName);
			l_FPSCameraController->AddYaw(-CInputManager::GetInputManager()->GetAxis("X_AXIS") * _ElapsedTime * 0.05f);
			l_FPSCameraController->AddPitch(CInputManager::GetInputManager()->GetAxis("Y_AXIS") * _ElapsedTime * 0.5f);

			l_FPSCameraController->Move(CInputManager::GetInputManager()->GetAxis("STRAFE"), CInputManager::GetInputManager()->GetAxis("MOVE_FWD"), false, _ElapsedTime);
		}
		break;
	}

	UABEngine.GetRenderManager()->SetUseDebugCamera(m_CurrentCamera_vision == 0);
	UABEngine.GetPhysXManager()->Update(_ElapsedTime);
	UABEngine.GetCameraControllerManager()->Update(_ElapsedTime);
	UABEngine.GetRenderManager()->SetUseDebugCamera(m_CurrentCamera_vision == 0);
	UABEngine.GetLayerManager()->Update(_ElapsedTime);
}

void CApplication::Render()
{
	UABEngine.GetRenderManager()->Render();
}


void CApplication::Init()
{
	UABEngine.Init();
	if (UABEngine.GetLevelLoaded() == "1" || UABEngine.GetLevelLoaded() == "3")
	{
		m_MainCameraName = "Camera001";
	}
	else
	{
		m_MainCameraName = "FPSCamera";
	}
	m_DebugCameraName = "SphericalCamera";
	UABEngine.GetCameraControllerManager()->ChooseMainCamera(m_MainCameraName);
	UABEngine.GetCameraControllerManager()->ChooseDebugCamera(m_DebugCameraName);
	m_FPSCamera = (CFPSCameraController*)UABEngine.GetCameraControllerManager()->GetMainCamera();
	m_SphericalCamera = (CSphericalCameraController*)UABEngine.GetCameraControllerManager()->GetMainCamera();
}
