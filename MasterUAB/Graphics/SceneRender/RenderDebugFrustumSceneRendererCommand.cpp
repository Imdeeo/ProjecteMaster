#include "RenderDebugFrustumSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"

#include "LevelManager\Level.h"

CRenderDebugFrustumSceneRendererCommand::CRenderDebugFrustumSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level){}

void CRenderDebugFrustumSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetRenderManager()->GetFrustum()->Render(&RenderManager);
#endif
}