#include "EnableAlphaBlendSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}


CEnableAlphaBlendSceneRendererCommand::~CEnableAlphaBlendSceneRendererCommand()
{
}

void CEnableAlphaBlendSceneRendererCommand::Execute(CRenderManager &_RenderManager){
	_RenderManager.EngableAlphaBlendState();
}