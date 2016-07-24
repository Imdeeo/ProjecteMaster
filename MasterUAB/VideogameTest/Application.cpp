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
#include "SoundManager\SoundManager.h"
#include "Camera\CameraControllerManager.h"

#include "no_sillywarnings_please.h"

CApplication::CApplication( CContextManager *_ContextManager)
	: m_BackgroundColor(.2f, .1f, .4f)
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


void CApplication::Update(float _ElapsedTime)
{	
	m_Timer+= _ElapsedTime;
	UABEngine.GetEffectManager()->m_SceneParameters.m_Time = m_Timer;

#ifdef _DEBUG

	gainput::InputMap* l_InputMap = UABEngine.GetInputManager()->GetMap();

	if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugReloadLua))
	{
		UABEngine.LuaIsReloaded();
		UABEngine.GetScriptManager()->RunFile("Data\\Lua\\init.lua");
		UtilsLog("Reloading Lua");
		UABEngine.GetScriptManager()->RunCode("mainLua(\"" + UABEngine.GetLevelLoaded() + "\")");
	}

	if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugToggleRenderLights))
		UABEngine.GetLightManager()->SwitchRenderLights();

	if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugChangeCamera))
		UABEngine.SwitchCamera();

	if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugChangeCameraVision))
		UABEngine.ChangeCameraVision();

	if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugChangeCameraControl))
	{
		int l_currentCameraCotnrol = UABEngine.GetCameraControllerManager()->GetCurrentCameraControl();
		l_currentCameraCotnrol++;
		l_currentCameraCotnrol = l_currentCameraCotnrol % 2;
		UABEngine.GetCameraControllerManager()->SetCurrentCameraControl(l_currentCameraCotnrol);
	}

	if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugToggleRenderCamera))
		m_RenderCameraCube = !m_RenderCameraCube;

	/*if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugMonsterIdle))
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
	if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugMonsterRun))
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
	if (l_InputMap->GetBoolWasDown(CInputManager::Actions::DebugMonsterHit))
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

#endif

	// ANTIGUO UPDATE
	/*if (_ElapsedTime > 0.0f){
		UABEngine.GetRenderManager()->SetUseDebugCamera(m_CurrentCamera_vision == 0);
		UABEngine.GetPhysXManager()->Update(_ElapsedTime);
		UABEngine.GetCameraControllerManager()->Update(_ElapsedTime);
		UABEngine.GetRenderManager()->SetUseDebugCamera(m_CurrentCamera_vision == 0);
		UABEngine.GetLayerManager()->Update(_ElapsedTime);
		UABEngine.GetScriptManager()->RunCode("luaUpdate(" + std::to_string(_ElapsedTime) + ")");
		const CCamera *l_CurrentCamera = UABEngine.GetRenderManager()->GetCurrentCamera();
		UABEngine.GetSoundManager()->Update(l_CurrentCamera);
	}*/

	UABEngine.Update(_ElapsedTime);
}

void CApplication::Render()
{
	UABEngine.GetRenderManager()->Render();
}


void CApplication::Init()
{
	UABEngine.Init();
//	CCharacterManager::GetInstance()->Load("Data\\level_"+UABEngine.GetLevelLoaded()+"\\characters.xml");
}
