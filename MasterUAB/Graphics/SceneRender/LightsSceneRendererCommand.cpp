#include "LightsSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"
#include "Effects\EffectManager.h"

#include "LevelManager\Level.h"

#include "Utils.h"

CLightsSceneRendererCommand::CLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :CSceneRendererCommand(TreeNode,_Level){}

void CLightsSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	UABEngine.GetEffectManager()->SetLightsConstants(MAX_LIGHTS_BY_SHADER,m_LevelId);

}
