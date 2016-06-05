#include "RenderDebugFrustumSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"

CRenderDebugFrustumSceneRendererCommand::CRenderDebugFrustumSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode){}

void CRenderDebugFrustumSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetRenderManager()->GetFrustum()->Render(&RenderManager);
#endif
}