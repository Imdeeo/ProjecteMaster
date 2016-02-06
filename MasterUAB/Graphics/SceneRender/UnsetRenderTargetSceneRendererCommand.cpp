#include "UnsetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	
}

void CUnsetRenderTargetSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	UABEngine.GetRenderManager()->UnsetRenderTargets();
}