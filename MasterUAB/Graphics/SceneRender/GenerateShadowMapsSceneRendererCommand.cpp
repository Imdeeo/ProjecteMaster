#include "GenerateShadowMapsSceneRendererCommand.h"

#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "Texture\DynamicTexture.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "LevelManager\LevelManager.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) :CSceneRendererCommand(TreeNode,_LevelId)
{
}


CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	CLightManager *l_LightManager=UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager();
	int n_lights = l_LightManager->GetResourcesVector().size();
	int count = 0;

	for (int i = 0; i < n_lights; i++)
	{
		CLight *l_Light=l_LightManager->GetResourceById(i);
		if(l_Light->GetEnabled() && l_Light->GetGenerateShadowMap() && l_Light->GetInsideFrustum()){
			l_Light->SetShadowMap(_RenderManager);
			_RenderManager.Clear(true, true);
			std::vector<CRenderableObjectsManager *> &l_Layers = l_Light->GetLayers();
			for(size_t j=0;j<l_Layers.size();++j)
				l_Layers[j]->Render(&_RenderManager);
			_RenderManager.GetContextManager()->UnsetRenderTargets();
			count++;
		}
	}
	printf("TOTAL DE SOMBRAS PINTADAS %i", count);
}