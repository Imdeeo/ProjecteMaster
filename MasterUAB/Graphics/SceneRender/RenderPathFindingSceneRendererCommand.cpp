#include "RenderPathFindingSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "Effects\EffectManager.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "IA\AStar.h"

CRenderPathFindingSceneRendererCommand::CRenderPathFindingSceneRendererCommand(tinyxml2::XMLElement* TreeNode) :CSceneRendererCommand(TreeNode)
{
}

CRenderPathFindingSceneRendererCommand::~CRenderPathFindingSceneRendererCommand()
{
}

void CRenderPathFindingSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetInstance()->GetAStarManager()->Render(&RenderManager);
#endif
}