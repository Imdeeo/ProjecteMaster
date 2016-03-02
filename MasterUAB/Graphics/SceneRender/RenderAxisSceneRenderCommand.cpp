#include "RenderAxisSceneRenderCommand.h"

#include "RenderManager\RenderManager.h"

CRenderAxisSceneRenderCommand::CRenderAxisSceneRenderCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}

CRenderAxisSceneRenderCommand::~CRenderAxisSceneRenderCommand()
{
}

void CRenderAxisSceneRenderCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->SetWorldMatrix(m44fIDENTITY);
	CEffectTechnique * l_ET = RenderManager.GetDebugRender()->GetDebugTechnique();
	CEffectManager::SetSceneConstants(l_ET);	
	RenderManager.GetDebugRender()->GetAxis()->Render(&RenderManager, l_ET, CEffectManager::GetRawData());
}