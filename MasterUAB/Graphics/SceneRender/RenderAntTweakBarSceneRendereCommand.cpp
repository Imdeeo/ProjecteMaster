#include "RenderAntTweakBarSceneRendereCommand.h"
#include "RenderManager\RenderManager.h"

CRenderAntTweakBarSceneRendererCommand::CRenderAntTweakBarSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId){}

CRenderAntTweakBarSceneRendererCommand::~CRenderAntTweakBarSceneRendererCommand(){}

void CRenderAntTweakBarSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.SetAntTweakBar();
}