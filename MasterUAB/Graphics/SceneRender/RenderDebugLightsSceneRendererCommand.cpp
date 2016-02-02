#include "RenderDebugLightsSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode){}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	UABEngine.GetLightManager()->Render(&RenderManager);
}