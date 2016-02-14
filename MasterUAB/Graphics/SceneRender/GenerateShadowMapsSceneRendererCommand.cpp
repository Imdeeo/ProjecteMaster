#include "GenerateShadowMapsSceneRendererCommand.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"

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
		Matrices view y projectionm, y SetRenderTarget de cada luz con el método SetShadowMap
		Clear Depth
		Pintar las capas de luces
	*/

	int n_lights = UABEngine.GetLightManager()->GetResourcesVector().size();
	for (size_t i = 0; i < n_lights; i++)
	{
		if(UABEngine.GetLightManager()->GetResourceById(i)->GetEnabled() && UABEngine.GetLightManager()->GetResourceById(i)->GetGenerateShadowMap()){
			UABEngine.GetLightManager()->GetResourceById(i)->SetShadowMap(_RenderManager);
			_RenderManager.Clear(false, true);
			UABEngine.GetLightManager()->GetResourceById(i)->GetShadowMap();
		}
	}
}