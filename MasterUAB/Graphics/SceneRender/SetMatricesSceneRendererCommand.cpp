#include "SetMatricesSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{

}

CSetMatricesSceneRendererCommand::~CSetMatricesSceneRendererCommand(){}

void CSetMatricesSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	// TODO
}