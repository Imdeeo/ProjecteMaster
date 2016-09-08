#include "RenderGUISceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "GUIManager.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_GUI = UABEngine.GetGUIManager();
}

CRenderGUISceneRendererCommand::~CRenderGUISceneRendererCommand(){
}

void CRenderGUISceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_GUI->Render(&RenderManager);
}