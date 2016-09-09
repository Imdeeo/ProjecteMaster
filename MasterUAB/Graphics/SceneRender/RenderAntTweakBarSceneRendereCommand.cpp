#include "RenderAntTweakBarSceneRendereCommand.h"
#include "RenderManager\RenderManager.h"

CRenderAntTweakBarSceneRendererCommand::CRenderAntTweakBarSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode){}

CRenderAntTweakBarSceneRendererCommand::~CRenderAntTweakBarSceneRendererCommand(){}

void CRenderAntTweakBarSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.SetAntTweakBar();
}