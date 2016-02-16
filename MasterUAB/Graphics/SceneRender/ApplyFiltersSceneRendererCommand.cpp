#include "ApplyFiltersSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Texture\DynamicTexture.h"
#include "Engine\UABEngine.h"
#include "Materials\MaterialParameter.h"

CApplyFiltersSceneRendererCommand::CApplyFiltersSceneRendererCommand(CXMLTreeNode &TreeNode) : CStagedTexturedSceneRendererCommand(TreeNode)
{

}

void CApplyFiltersSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	CContextManager *l_ContextManager = _RenderManager.GetContextManager();
	ID3D11DepthStencilView *l_DepthStencilView = m_DynamicTextures.empty() ? NULL : (m_DynamicTextures[0]->GetDepthStencilView());

	for (size_t i = 0; i < m_DynamicTextures.size(); ++i)
	{
		m_Materials[i]->Apply();
		//_RenderManager.Clear(false,true);
		l_ContextManager->GetDeviceContext()->OMSetRenderTargets(1, &m_RenderTargetViews[i], nullptr);
		if (i == 0)
			_RenderManager.DrawScreenQuad(m_Materials[i]->GetRenderableObjectTechnique()->GetEffectTechnique(), m_StagedTextures[0].m_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
		else
			_RenderManager.DrawScreenQuad(m_Materials[i]->GetRenderableObjectTechnique()->GetEffectTechnique(), m_DynamicTextures[i - 1], 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));		
	}
}