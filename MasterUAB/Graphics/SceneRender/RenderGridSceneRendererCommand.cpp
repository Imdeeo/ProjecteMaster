#include "RenderGridSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "Effects\EffectManager.h"
#include "RenderableObjects\RenderableVertexs.h"

#include "LevelManager\Level.h"

CRenderGridSceneRendererCommand::CRenderGridSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :CSceneRendererCommand(TreeNode,_Level)
{
}

CRenderGridSceneRendererCommand::~CRenderGridSceneRendererCommand()
{
}

void CRenderGridSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	RenderManager.GetContextManager()->SetWorldMatrix(m44fIDENTITY);
	CEffectTechnique * l_ET = RenderManager.GetDebugRender()->GetDebugTechnique();
	CEffectManager::SetSceneConstants(l_ET);
	RenderManager.GetDebugRender()->GetBigGrid()->RenderIndexed(&RenderManager,l_ET,CEffectManager::GetRawData());
#endif
}