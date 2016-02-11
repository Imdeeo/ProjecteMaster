#include "ApplyFiltersSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

CApplyFiltersSceneRendererCommand::CApplyFiltersSceneRendererCommand(CXMLTreeNode &TreeNode) : CStagedTexturedSceneRendererCommand(TreeNode)
{

}

void CApplyFiltersSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	for (size_t i = 0; i < m_StagedTextures.size(); ++i)
		m_StagedTextures[i].Activate();

	for (size_t i = 0; i < m_DynamicTextures.size(); ++i)
	{
	}
}