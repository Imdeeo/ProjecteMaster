#include "DeferredShadingSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Engine\UABEngine.h"

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");
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

	for (int j = 0; j < UABEngine.GetLightManager()->GetResourcesVector().size(); ++j)
	{
		UABEngine.GetEffectManager()->SetOneLightConstants(UABEngine.GetLightManager()->GetResourceById(j));
		for (int i = 0; i < m_StagedTextures.size(); ++i)
				m_StagedTextures[i].Activate();
		_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	}
}		
