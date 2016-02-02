#include "RenderAntTweakBarSceneRendereCommand.h"
#include "RenderManager\RenderManager.h"

CRenderAntTweakBarSceneRendererCommand::CRenderAntTweakBarSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode){}

CRenderAntTweakBarSceneRendererCommand::~CRenderAntTweakBarSceneRendererCommand(){}

void CRenderAntTweakBarSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.SetAntTweakBar();
}