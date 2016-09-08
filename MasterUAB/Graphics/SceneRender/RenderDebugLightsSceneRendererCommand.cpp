#include "RenderDebugLightsSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode){}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetLightManager()->Render(&RenderManager);
#endif
}