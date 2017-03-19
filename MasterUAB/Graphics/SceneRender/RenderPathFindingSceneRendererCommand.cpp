#include "RenderPathFindingSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "Effects\EffectManager.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "IA\AStarManager.h"

#include "LevelManager\LevelManager.h"

CRenderPathFindingSceneRendererCommand::CRenderPathFindingSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :CSceneRendererCommand(TreeNode,_Level)
{
}

CRenderPathFindingSceneRendererCommand::~CRenderPathFindingSceneRendererCommand()
{
}

void CRenderPathFindingSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetAStarManager()->GetResource(m_LevelId)->Render(&RenderManager);
#endif
}