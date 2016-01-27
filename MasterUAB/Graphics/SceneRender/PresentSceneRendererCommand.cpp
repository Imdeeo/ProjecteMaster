#include "PresentSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{

}

CPresentSceneRendererCommand::~CPresentSceneRendererCommand(){}

void CPresentSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	// TODO
}