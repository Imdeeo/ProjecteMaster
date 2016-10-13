#include "RenderDebugLightsSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"
#include "LevelManager\LevelManager.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode, _LevelId)
{
	m_Debug = TreeNode->GetBoolProperty("debug", true);
}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	if (m_Debug)
		UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->RenderAux(&RenderManager);
	else
	{
		UABEngine.GetRenderManager()->GetDeviceContext()->RSSetState(UABEngine.GetRenderManager()->GetContextManager()->GetRasterizerState(CContextManager::RS_SOLID_NO_CULL));
		UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->Render(&RenderManager);
		UABEngine.GetRenderManager()->GetDeviceContext()->RSSetState(UABEngine.GetRenderManager()->GetContextManager()->GetRasterizerState(CContextManager::RS_SOLID_BACK_CULL));
	}
}