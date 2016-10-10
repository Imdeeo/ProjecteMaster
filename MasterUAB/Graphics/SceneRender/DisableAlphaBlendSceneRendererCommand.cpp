#include "DisableAlphaBlendSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"


CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId)
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
