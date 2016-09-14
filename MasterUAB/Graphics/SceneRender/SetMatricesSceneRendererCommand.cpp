#include "SetMatricesSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode){}
CSetMatricesSceneRendererCommand::~CSetMatricesSceneRendererCommand(){}

void CSetMatricesSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.SetMatrixViewProjection();
}