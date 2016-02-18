#include "GenerateShadowMapsSceneRendererCommand.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "Texture\DynamicTexture.h"
#include "RenderableObjects\RenderableObjectsManager.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}


CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	CLightManager *l_LightManager=UABEngine.GetLightManager();
	int n_lights = l_LightManager->GetResourcesVector().size();
	for (size_t i = 0; i < n_lights; i++)
	{
		CLight *l_Light=l_LightManager->GetResourceById(i);
		if(l_Light->GetEnabled() && l_Light->GetGenerateShadowMap()){
			l_Light->SetShadowMap(_RenderManager);
			_RenderManager.Clear(true, true);
			std::vector<CRenderableObjectsManager *> &l_Layers = l_Light->GetLayers();
			for(size_t j=0;j<l_Layers.size();++j)
				l_Layers[j]->Render(&_RenderManager);
			_RenderManager.GetContextManager()->UnsetRenderTargets();
		}
	}
}