#include "RenderDebugLayerSceneRendererCommand.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectsManager.h"


CRenderDebugLayerSceneRendererCommand::CRenderDebugLayerSceneRendererCommand(CXMLTreeNode &TreeNode):CSceneRendererCommand(TreeNode)
{
	m_Layer = UABEngine.GetLayerManager()->GetLayer(TreeNode);
	Setactive(TreeNode.GetBoolProperty("active", false));
}

CRenderDebugLayerSceneRendererCommand::~CRenderDebugLayerSceneRendererCommand()
{
	//CHECKED_RELEASE(m_Layer);
}

void CRenderDebugLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_Layer->RenderDebug(&RenderManager);
}