#include "ApplyFiltersSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "Texture\DynamicTextureManager.h"
#include "Texture\DynamicTexture.h"
#include "Engine\UABEngine.h"

#include "Materials\Material.h"
#include "Materials\MaterialParameter.h"

#include "RenderableObjects\RenderableObjectTechnique.h"

#include <d3d11.h>

CApplyFiltersSceneRendererCommand::CApplyFiltersSceneRendererCommand(CXMLTreeNode &TreeNode) : CStagedTexturedSceneRendererCommand(TreeNode)
{

}

void CApplyFiltersSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	CContextManager *l_ContextManager = _RenderManager.GetContextManager();
	ID3D11DepthStencilView *l_DepthStencilView = m_DynamicTextures.empty() ? NULL : (m_DynamicTextures[0]->GetTexture()->GetDepthStencilView());

	D3D11_VIEWPORT *l_CurrentViewport=_RenderManager.GetContextManager()->getViewPort();
	
	for (size_t i = 0; i < m_DynamicTextures.size(); ++i)
	{
		D3D11_VIEWPORT l_Viewport;
		l_Viewport.TopLeftX=0.0f;
		l_Viewport.TopLeftY=0.0f;
		l_Viewport.MinDepth=0.0f;
		l_Viewport.MaxDepth=1.0f;
		l_Viewport.Width=m_DynamicTextures[i]->GetTexture()->GetWidth();
		l_Viewport.Height=m_DynamicTextures[i]->GetTexture()->GetHeight();
		_RenderManager.GetContextManager()->setViewPort(&l_Viewport);

		m_DynamicTextures[i]->GetMaterial()->Apply();		
		l_ContextManager->SetRenderTargets(1, &m_RenderTargetViews[i], nullptr);
		_RenderManager.DrawScreenQuad(m_DynamicTextures[i]->GetMaterial()->GetRenderableObjectTechnique()->GetEffectTechnique(),
			i==0 ? m_StagedTextures[0].m_Texture : m_DynamicTextures[i-1]->GetTexture(), 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	}
	_RenderManager.GetContextManager()->setViewPort(l_CurrentViewport);
}