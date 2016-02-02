#include "RenderLayerSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_Layer = UABEngine.GetLayerManager()->GetLayer(TreeNode);
	Setactive(TreeNode.GetBoolProperty("active",false));
}

CRenderLayerSceneRendererCommand::~CRenderLayerSceneRendererCommand(){}

void CRenderLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_Layer->Render(&RenderManager);
}