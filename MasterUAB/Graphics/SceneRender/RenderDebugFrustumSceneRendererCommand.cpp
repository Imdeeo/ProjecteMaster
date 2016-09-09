#include "RenderDebugFrustumSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"

CRenderDebugFrustumSceneRendererCommand::CRenderDebugFrustumSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode){}

void CRenderDebugFrustumSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetRenderManager()->GetFrustum()->Render(&RenderManager);
#endif
}