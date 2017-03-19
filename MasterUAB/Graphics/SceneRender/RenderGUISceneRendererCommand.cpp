#include "RenderGUISceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "GUIManager.h"

#include "LevelManager\Level.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level)
{
	m_GUI = UABEngine.GetGUIManager();
}

CRenderGUISceneRendererCommand::~CRenderGUISceneRendererCommand(){
}

void CRenderGUISceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_GUI->Render(&RenderManager);
}