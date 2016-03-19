#include "Application.h"

#include "Math\Matrix44.h"
#include "Math\Vector4.h"

#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "InputManager\InputManager.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "DebugHelper\DebugHelper.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObject.h"
#include "Effects\EffectManager.h"
#include "Layers\LayerManager.h"
#include "Lights\LightManager.h"
#include "PhysXManager\PhysXManager.h"
#include "ScriptManager\ScriptManager.h"

#include "Characters\CharacterManager.h"
#include "Camera\CameraControllerManager.h"

CApplication::CApplication( CContextManager *_ContextManager)
	: m_BackgroundColor(.2f, .1f, .4f)
	, m_CurrentCamera_vision(1)
	, m_RenderCameraCube(false)
	, m_Timer(0)
{
	CDebugHelper::GetDebugHelper()->Log("CApplication::CApplication");

	UABEngine.GetRenderManager()->SetContextManager(_ContextManager);
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
	UABEngine.GetCameraControllerManager()->SetCurrentCameraControl(m_CurrentCamera_vision);
}

void CApplication::Update(float _ElapsedTime)
{	
	m_Timer+= _ElapsedTime;
	UABEngine.GetEffectManager()->m_SceneParameters.m_Time = m_Timer;

	if(CInputManager::GetInputManager()->IsActionActive("RELOAD_LUA"))
	{
		UABEngine.GetScriptManager()->RunFile("Data\\Lua\\init.lua");
		UtilsLog("Reloading Lua");
		UABEngine.GetScriptManager()->RunCode("mainLua()");
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
		int l_currentCameraCotnrol = UABEngine.GetCameraControllerManager()->GetCurrentCameraControl();
		l_currentCameraCotnrol++;
		l_currentCameraCotnrol = l_currentCameraCotnrol % 2;
		UABEngine.GetCameraControllerManager()->SetCurrentCameraControl(l_currentCameraCotnrol);
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

	UABEngine.GetRenderManager()->SetUseDebugCamera(m_CurrentCamera_vision == 0);
	UABEngine.GetPhysXManager()->Update(_ElapsedTime);
	UABEngine.GetCameraControllerManager()->Update(_ElapsedTime);
	UABEngine.GetRenderManager()->SetUseDebugCamera(m_CurrentCamera_vision == 0);
	UABEngine.GetLayerManager()->Update(_ElapsedTime);
	UABEngine.GetScriptManager()->RunCode("luaUpdate(" + std::to_string(_ElapsedTime) + ")");
}

void CApplication::Render()
{
	UABEngine.GetRenderManager()->Render();
}


void CApplication::Init()
{
	UABEngine.Init();	
	CCharacterManager::GetInstance()->Load("Data\\level_"+UABEngine.GetLevelLoaded()+"\\characters.xml");
}
