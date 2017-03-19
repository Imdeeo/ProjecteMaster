#include "DeferredShadingSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"
#include "Effects\EffectManager.h"

#include "MutexManager\MutexManager.h"

#include "LevelManager\LevelManager.h"

#include "RenderableObjects\RenderableObjectTechnique.h"

#include <d3d11.h>

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :CStagedTexturedSceneRendererCommand(TreeNode,_Level)
{
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");
}


CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
}

void CDeferredShadingSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	std::mutex *l_DeviceContextMutex = &(UABEngine.GetMutexManager()->g_DeviceContextMutex);
	m_EnabledAlphaBlendState = _RenderManager.GetContextManager()->GetBlendState(CContextManager::BLEND_DEFERRED);
 	if (m_EnabledAlphaBlendState == nullptr)
		return;
	
	l_DeviceContextMutex->lock();
	_RenderManager.GetDeviceContext()->OMSetBlendState(m_EnabledAlphaBlendState, NULL, 0xffffffff);
	l_DeviceContextMutex->unlock();

	for (size_t i = 0; i < m_StagedTextures.size(); ++i)
				m_StagedTextures[i].Activate();

	int count = 0;

	for (size_t j = 0; j < UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->GetResourcesVector().size(); ++j)
	{
		CLight *l_Light = UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->GetResourceById(j);
		if (!l_Light->GetEnabled() || !l_Light->GetInsideFrustum()) {
			continue;
		}

		count++;
		UABEngine.GetEffectManager()->SetLightConstants(0, l_Light);

		CEffectTechnique* l_EffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();
		ID3D11Buffer *l_LightConstantBufferVS = l_EffectTechnique->GetVertexShader()->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_LightConstantBufferPS = l_EffectTechnique->GetPixelShader()->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);

		l_DeviceContextMutex->lock();
		_RenderManager.GetDeviceContext()->UpdateSubresource(l_LightConstantBufferVS, 0, NULL, &(CEffectManager::m_LightParameters), 0, 0);
		_RenderManager.GetDeviceContext()->UpdateSubresource(l_LightConstantBufferPS, 0, NULL, &(CEffectManager::m_LightParameters), 0, 0);
		l_DeviceContextMutex->unlock();

		_RenderManager.DrawScreenQuad(l_EffectTechnique, NULL, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	}
	printf("TOTAL DE LUCES PINTADAS %i", count);

	l_DeviceContextMutex->lock();
	_RenderManager.GetContextManager()->GetDeviceContext()->OMSetBlendState(NULL, NULL, 0xffffffff);
	l_DeviceContextMutex->unlock();
}	
