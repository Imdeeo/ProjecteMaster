#include "RenderDebugLayerSceneRendererCommand.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "Layers\LayerManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"


CRenderDebugLayerSceneRendererCommand::CRenderDebugLayerSceneRendererCommand(tinyxml2::XMLElement* TreeNode) :CSceneRendererCommand(TreeNode)
{
	m_Layer = UABEngine.GetLayerManager()->GetLayer(TreeNode);
	SetActive(TreeNode->GetBoolProperty("active", false));
}

CRenderDebugLayerSceneRendererCommand::~CRenderDebugLayerSceneRendererCommand()
{
	//CHECKED_RELEASE(m_Layer);
}

void CRenderDebugLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	m_Layer->RenderDebug(&RenderManager);
#endif
}