#include "DisableAlphaBlendSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"

#include "LevelManager\Level.h"


CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level)
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
