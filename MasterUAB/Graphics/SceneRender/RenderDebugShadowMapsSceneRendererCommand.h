#ifndef RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_h
#define RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_h

#include "SceneRendererCommand.h"
#include "RenderableObjects\RenderableObjectTechnique.h"

class CRenderDebugShadowMapsSceneRendererCommand :
	public CSceneRendererCommand
{
private:
	float m_Width;
	float m_Offset;
	float m_Light;
	CRenderableObjectTechnique	*m_RenderableObjectTechnique;
public:
	CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CRenderDebugShadowMapsSceneRendererCommand();

	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_h