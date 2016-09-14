#include "DisableAlphaBlendSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"


CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode)
{
}


CDisableAlphaBlendSceneRendererCommand::~CDisableAlphaBlendSceneRendererCommand()
{
}

void CDisableAlphaBlendSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.DisableAlphaBlendState();
	//RM.DisableAlphaBlendState()
}
