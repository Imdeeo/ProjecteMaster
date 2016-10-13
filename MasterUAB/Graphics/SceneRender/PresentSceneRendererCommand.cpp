#include "PresentSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId){}
CPresentSceneRendererCommand::~CPresentSceneRendererCommand(){}

void CPresentSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.Present();
}