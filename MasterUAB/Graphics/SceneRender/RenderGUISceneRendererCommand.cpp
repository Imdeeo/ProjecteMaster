#include "RenderGUISceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "GUIManager.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_GUI = UABEngine.GetGUIManager();
}

CRenderGUISceneRendererCommand::~CRenderGUISceneRendererCommand(){
}

void CRenderGUISceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_GUI->Render(&RenderManager);
}