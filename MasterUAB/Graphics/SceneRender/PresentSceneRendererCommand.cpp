#include "PresentSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode){}
CPresentSceneRendererCommand::~CPresentSceneRendererCommand(){}

void CPresentSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.Present();
}