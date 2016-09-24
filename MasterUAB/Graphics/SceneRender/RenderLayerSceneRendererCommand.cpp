#include "RenderLayerSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"
#include "LevelManager\LevelManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode)
{
	const char * l_layerExist = TreeNode->GetPszProperty("layer", "");
	m_Layer = UABEngine.GetLevelManager()->GetCompleteLayer(l_layerExist);
	SetActive(TreeNode->GetBoolProperty("active",false));
}

CRenderLayerSceneRendererCommand::~CRenderLayerSceneRendererCommand(){}

void CRenderLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	for (size_t i = 0; i < m_Layer.size(); i++)
	{
		m_Layer[i]->Render(&RenderManager);
	}
}