#include "ClearSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "XML\XMLTreeNode.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_RenderTarget = TreeNode.GetBoolProperty("render_target");
	m_DepthStencil = TreeNode.GetBoolProperty("depth_stencil");
}

CClearSceneRendererCommand::~CClearSceneRendererCommand(){}

void CClearSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.Clear(m_RenderTarget, m_DepthStencil);
}