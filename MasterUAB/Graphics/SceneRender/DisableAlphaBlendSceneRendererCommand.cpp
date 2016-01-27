#include "DisableAlphaBlendSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"


CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
}


CDisableAlphaBlendSceneRendererCommand::~CDisableAlphaBlendSceneRendererCommand()
{
}

void CDisableAlphaBlendSceneRendererCommand::Execute(CRenderManager &RM)
{
	//RM.DisableAlphaBlendState()
}
