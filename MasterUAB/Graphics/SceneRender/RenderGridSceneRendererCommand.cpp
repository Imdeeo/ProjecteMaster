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
	RenderManager.GetContextManager()->SetWorldMatrix(m44fIDENTITY);
	CEffectTechnique * l_ET = RenderManager.GetDebugRender()->GetDebugTechnique();
	CEffectManager::SetSceneConstants(l_ET);
	RenderManager.GetDebugRender()->GetBigGrid()->RenderIndexed(&RenderManager,l_ET,CEffectManager::GetRawData());
}