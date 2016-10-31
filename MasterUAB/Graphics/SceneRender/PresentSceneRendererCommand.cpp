#include "PresentSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

#include "LevelManager\Level.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level){}
CPresentSceneRendererCommand::~CPresentSceneRendererCommand(){}

void CPresentSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.Present();
}