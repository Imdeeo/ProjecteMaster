#include "RenderLayerSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"
#include "Layers\LayerManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "XML\XMLTreeNode.h"

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_Layer = UABEngine.GetLayerManager()->GetLayer(TreeNode);
	SetActive(TreeNode.GetBoolProperty("active",false));
}

CRenderLayerSceneRendererCommand::~CRenderLayerSceneRendererCommand(){}

void CRenderLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_Layer->Render(&RenderManager);
}