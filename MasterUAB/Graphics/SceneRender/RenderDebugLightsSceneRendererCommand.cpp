#include "RenderDebugLightsSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"
#include "LevelManager\LevelManager.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "MutexManager\MutexManager.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode, _Level)
{
	m_Debug = TreeNode->GetBoolProperty("debug", true);
}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	if (m_Debug)
		UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->RenderAux(&RenderManager);
	else
	{
		std::mutex * l_DeviceContextMutex = &(UABEngine.GetMutexManager()->g_DeviceContextMutex);
		l_DeviceContextMutex->lock();
		UABEngine.GetRenderManager()->GetDeviceContext()->RSSetState(UABEngine.GetRenderManager()->GetContextManager()->GetRasterizerState(CContextManager::RS_SOLID_NO_CULL));
		l_DeviceContextMutex->unlock();
		UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->Render(&RenderManager);
		l_DeviceContextMutex->lock();
		UABEngine.GetRenderManager()->GetDeviceContext()->RSSetState(UABEngine.GetRenderManager()->GetContextManager()->GetRasterizerState(CContextManager::RS_SOLID_BACK_CULL));
		l_DeviceContextMutex->unlock();
	}
}