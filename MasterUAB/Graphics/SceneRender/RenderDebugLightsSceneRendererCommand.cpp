#include "RenderDebugLightsSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_Debug = TreeNode->GetBoolProperty("debug", true);
}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	if (m_Debug)
		UABEngine.GetLightManager()->RenderAux(&RenderManager);
	else
	{
		UABEngine.GetRenderManager()->GetDeviceContext()->RSSetState(UABEngine.GetRenderManager()->GetContextManager()->GetRasterizerState(CContextManager::RS_SOLID_NO_CULL));
		UABEngine.GetLightManager()->Render(&RenderManager);
		UABEngine.GetRenderManager()->GetDeviceContext()->RSSetState(UABEngine.GetRenderManager()->GetContextManager()->GetRasterizerState(CContextManager::RS_SOLID_BACK_CULL));
	}
}