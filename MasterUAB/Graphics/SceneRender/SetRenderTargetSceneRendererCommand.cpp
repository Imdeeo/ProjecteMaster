#include "SetRenderTargetSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Texture\DynamicTextureManager.h"
#include "Texture\DynamicTexture.h"
#include "Engine\UABEngine.h"
#include "ContextManager\ContextManager.h"

#include <d3d11.h>

CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{
}

CSetRenderTargetSceneRendererCommand::~CSetRenderTargetSceneRendererCommand()
{
}

void CSetRenderTargetSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	CContextManager *l_ContextManager=_RenderManager.GetContextManager();
	ID3D11DepthStencilView *l_DepthStencilView = m_DynamicTextures.empty() ? NULL : (m_DynamicTextures[0]->GetTexture()->GetDepthStencilView());
	l_ContextManager->SetRenderTargets((UINT)m_DynamicTextures.size(), &m_RenderTargetViews[0], l_DepthStencilView==NULL ? l_ContextManager->GetDepthStencilView() : l_DepthStencilView);
}
