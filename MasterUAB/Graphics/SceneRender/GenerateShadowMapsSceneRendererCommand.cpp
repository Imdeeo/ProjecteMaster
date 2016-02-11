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
	/*Para ello estableceremos los valores de c�mara seg�n la luz, es decir crearemos
	la matriz de View y de Projection a trav�s de los par�metros de la luz en el
	m�todo SetShadowMap.

	Nos guardaremos las matrices de ShadowProjection y de ShadowView seg�n
	nuestras matrices de View y de Projection que ser�n utilizadas m�s tarde al
	renderizar la luz que genera la sombra.

	Una vez calculadas las matrices nos disponemos a renderizar la escena
	estableciendo nuestra textura como RenderTarget. Renderizamos nuestra escena
	del Proc y desestablecemos nuestra textura como RenderTarget.*/
}