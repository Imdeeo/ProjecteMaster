#include "GenerateShadowMapsSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}


CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	//TODO
	//foreach light in lights, light->SetShadowMap();
	/*Para ello estableceremos los valores de cámara según la luz, es decir crearemos
	la matriz de View y de Projection a través de los parámetros de la luz en el
	método SetShadowMap.

	Nos guardaremos las matrices de ShadowProjection y de ShadowView según
	nuestras matrices de View y de Projection que serán utilizadas más tarde al
	renderizar la luz que genera la sombra.

	Una vez calculadas las matrices nos disponemos a renderizar la escena
	estableciendo nuestra textura como RenderTarget. Renderizamos nuestra escena
	del Proc y desestablecemos nuestra textura como RenderTarget.*/
}