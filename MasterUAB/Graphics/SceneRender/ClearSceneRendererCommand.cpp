#include "ClearSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "XML\XMLTreeNode.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	// TODO
	/*m_DepthStencil = TreeNode.GetPszProperty("name");
	m_RenderTarget = TreeNode.GetPszProperty("name");*/
}

CClearSceneRendererCommand::~CClearSceneRendererCommand(){}

void CClearSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.Clear();
}