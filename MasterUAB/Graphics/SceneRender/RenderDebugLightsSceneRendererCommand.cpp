#include "RenderDebugLightsSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"
#include "LevelManager\LevelManager.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId){}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->Render(&RenderManager);
#endif
}