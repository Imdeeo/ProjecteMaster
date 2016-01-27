#include "RenderGridSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"

CRenderGridSceneRendererCommand::CRenderGridSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}


CRenderGridSceneRendererCommand::~CRenderGridSceneRendererCommand()
{
}

void CRenderGridSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	//TODO RenderGrid
}