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
	_RenderManager.GetContextManager()->GetDeviceContext()->OMSetBlendState(m_EnabledAlphaBlendState, NULL, 0xffffffff);

	int l_ShadowmapStageId = m_StagedTextures.size();

	for (int i = 0; i < m_StagedTextures.size(); ++i)
				m_StagedTextures[i].Activate();

	for (int j = 0; j < UABEngine.GetLightManager()->GetResourcesVector().size(); ++j)
	{
		CLight *l_Light = UABEngine.GetLightManager()->GetResourceById(j);
		UABEngine.GetEffectManager()->SetLightConstants(0, l_Light);
		l_Light->GetShadowMap()->Activate(l_ShadowmapStageId);
		_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	}
	_RenderManager.GetContextManager()->GetDeviceContext()->OMSetBlendState(NULL, NULL, 0xffffffff);
}	
