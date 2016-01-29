#include "DeferredShadingSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{

}


CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
}

void CDeferredShadingSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	m_EnabledAlphaBlendState = _RenderManager.GetContextManager()->GetBlendState(CContextManager::BLEND_DEFERRED);
	if (m_EnabledAlphaBlendState == nullptr)
		return;
	Vect4f v(1, 1, 1, 1);
	_RenderManager.GetContextManager()->GetDeviceContext()->OMSetBlendState(m_EnabledAlphaBlendState,&v.x,0xffffffff);

	for (int i = 0; i < m_StagedTextures.size(); i++)
	{
		_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), m_StagedTextures[i].m_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	}
	//TODO
}		
