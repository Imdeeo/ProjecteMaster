#include "GenerateShadowMapsSceneRendererCommand.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "Texture\DynamicTexture.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}


CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	/*
		por cada luz si está activada y genera shadowmap
		Matrices view y projectionm de cada luz con el método SetShadowMap
		SetRenderTarget
		Clear Depth
		Pintar las capas de luces
	*/

	int n_lights = UABEngine.GetLightManager()->GetResourcesVector().size();
	for (size_t i = 0; i < n_lights; i++)
	{
		if(UABEngine.GetLightManager()->GetResourceById(i)->GetEnabled() && UABEngine.GetLightManager()->GetResourceById(i)->GetGenerateShadowMap()){
			UABEngine.GetLightManager()->GetResourceById(i)->SetShadowMap(_RenderManager);
			_RenderManager.SetMatrixViewProjection();
			_RenderManager.GetContextManager()->UnsetRenderTargets();
			/*CContextManager *l_ContextManager = _RenderManager.GetContextManager();
			CDynamicTexture *l_DynamicTexture = new CDynamicTexture("shadowmap", 1024, 1024, false);
			ID3D11DepthStencilView *l_DepthStencilView = l_DynamicTexture->GetDepthStencilView();
			l_ContextManager->SetRenderTargets((UINT)1, l_DynamicTexture->GetRenderTargetView, l_DepthStencilView == NULL ? l_ContextManager->GetDepthStencilView() : l_DepthStencilView);
			l_DynamicTexture->Activate(1);*/
			_RenderManager.Clear(false, true);
			UABEngine.GetLightManager()->GetResourceById(i)->GetShadowMap()->Activate(0);
			_RenderManager.DrawScreenQuad(UABEngine.GetRenderableObjectTechniqueManager()->GetResource("2d")->GetEffectTechnique(), NULL, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
		}
	}
}