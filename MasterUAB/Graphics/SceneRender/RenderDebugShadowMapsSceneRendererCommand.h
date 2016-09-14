#ifndef RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_h
#define RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_h

#include "SceneRendererCommand.h"
class CRenderableObjectTechnique;

class CRenderDebugShadowMapsSceneRendererCommand :
	public CSceneRendererCommand
{
private:
	float m_Width;
	float m_Offset;
	int m_Light;
	CRenderableObjectTechnique	*m_RenderableObjectTechnique;
public:
	CRenderDebugShadowMapsSceneRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CRenderDebugShadowMapsSceneRendererCommand();

	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_h