#include "RenderDebugFrustumSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"

CRenderDebugFrustumSceneRendererCommand::CRenderDebugFrustumSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId){}

void CRenderDebugFrustumSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetRenderManager()->GetFrustum()->Render(&RenderManager);
#endif
}