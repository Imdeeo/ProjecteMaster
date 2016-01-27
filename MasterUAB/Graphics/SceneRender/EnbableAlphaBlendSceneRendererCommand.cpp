#include "EnbableAlphaBlendSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"

CEnbableAlphaBlendSceneRendererCommand::CEnbableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}


CEnbableAlphaBlendSceneRendererCommand::~CEnbableAlphaBlendSceneRendererCommand()
{
}

void CEnbableAlphaBlendSceneRendererCommand::Execute(CRenderManager &_RenderManager){
	_RenderManager.EngableAlphaBlendState();
}