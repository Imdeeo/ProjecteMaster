#include "RenderGUISceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "GUIManager.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId)
{
	m_GUI = UABEngine.GetGUIManager();
}

CRenderGUISceneRendererCommand::~CRenderGUISceneRendererCommand(){
}

void CRenderGUISceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_GUI->Render(&RenderManager);
}