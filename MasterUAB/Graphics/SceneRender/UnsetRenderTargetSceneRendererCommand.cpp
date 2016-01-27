#include "UnsetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	
}

void CUnsetRenderTargetSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	//TODO
}