#ifndef GENERATE_SHADOW_MAP_SCENE_RENDERER_COMMAND
#define GENERATE_SHADOW_MAP_SCENE_RENDERER_COMMAND
#include "SceneRendererCommand.h"
class CGenerateShadowMapsSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CGenerateShadowMapsSceneRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CGenerateShadowMapsSceneRendererCommand();

	void Execute(CRenderManager &_RenderManager);
};
#endif