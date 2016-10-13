#include "RenderDebugLightsSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"
<<<<<<< HEAD
#include "LevelManager\LevelManager.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId){}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->Render(&RenderManager);
#endif
=======
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
>>>>>>> develop
}