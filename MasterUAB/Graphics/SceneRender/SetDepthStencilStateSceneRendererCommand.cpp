#include "SetDepthStencilStateSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

CSetDepthStencilStateSceneRendererCommand::CSetDepthStencilStateSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{

}

CSetDepthStencilStateSceneRendererCommand::~CSetDepthStencilStateSceneRendererCommand()
{

}

void CSetDepthStencilStateSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	// TODO
}