#include "SetMatricesSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId){}
CSetMatricesSceneRendererCommand::~CSetMatricesSceneRendererCommand(){}

void CSetMatricesSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.SetMatrixViewProjection();
}