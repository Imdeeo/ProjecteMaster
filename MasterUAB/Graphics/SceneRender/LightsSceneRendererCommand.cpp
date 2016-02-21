#include "LightsSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"
#include "Utils.h"

CLightsSceneRendererCommand::CLightsSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode){}

void CLightsSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	UABEngine.GetEffectManager()->SetLightsConstants(MAX_LIGHTS_BY_SHADER);

}
