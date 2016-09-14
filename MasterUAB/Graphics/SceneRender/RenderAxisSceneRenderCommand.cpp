#include "RenderAxisSceneRenderCommand.h"

#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "Effects\EffectManager.h"
#include "RenderableObjects\RenderableVertexs.h"

CRenderAxisSceneRenderCommand::CRenderAxisSceneRenderCommand(tinyxml2::XMLElement* TreeNode) :CSceneRendererCommand(TreeNode)
{
}

CRenderAxisSceneRenderCommand::~CRenderAxisSceneRenderCommand()
{
}

void CRenderAxisSceneRenderCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	RenderManager.GetContextManager()->SetWorldMatrix(m44fIDENTITY);
	CEffectTechnique * l_ET = RenderManager.GetDebugRender()->GetDebugTechnique();
	CEffectManager::SetSceneConstants(l_ET);	
	RenderManager.GetDebugRender()->GetAxis()->Render(&RenderManager, l_ET, CEffectManager::GetRawData());
#endif
}