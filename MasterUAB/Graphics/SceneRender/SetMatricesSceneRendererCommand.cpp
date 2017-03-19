#include "SetMatricesSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

#include "LevelManager\Level.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level){}
CSetMatricesSceneRendererCommand::~CSetMatricesSceneRendererCommand(){}

void CSetMatricesSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.SetMatrixViewProjection();
}