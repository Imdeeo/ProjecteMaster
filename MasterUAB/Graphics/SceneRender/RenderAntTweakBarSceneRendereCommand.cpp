#include "RenderAntTweakBarSceneRendereCommand.h"
#include "RenderManager\RenderManager.h"

#include "LevelManager\Level.h"

CRenderAntTweakBarSceneRendererCommand::CRenderAntTweakBarSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level){}

CRenderAntTweakBarSceneRendererCommand::~CRenderAntTweakBarSceneRendererCommand(){}

void CRenderAntTweakBarSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.SetAntTweakBar();
}