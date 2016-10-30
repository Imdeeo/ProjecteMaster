#include "RenderLayerSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectsManager.h"

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level)
{
	const char * l_layerExist = TreeNode->GetPszProperty("layer", "");
	m_Layer = UABEngine.GetLevelManager()->GetCompleteLayer(l_layerExist);
	SetActive(TreeNode->GetBoolProperty("active",false));
}

CRenderLayerSceneRendererCommand::~CRenderLayerSceneRendererCommand(){}

void CRenderLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	for (size_t i = 0; i < m_Layer->size(); i++)
	{
		if ((*m_Layer)[i]->m_Visible)
		{
			(*m_Layer)[i]->m_Layer->Render(&RenderManager);
		}
	}
}