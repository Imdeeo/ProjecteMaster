#include "UnsetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	
}

void CUnsetRenderTargetSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->UnsetRenderTargets();
}