#include "RenderLayerSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
//#include "RenderableObjects\RenderableObjectsManager.h"

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{

}

CRenderLayerSceneRendererCommand::~CRenderLayerSceneRendererCommand(){}

void CRenderLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	// TODO
}