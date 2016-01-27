#ifndef RENDER_DEBUG_LAYER_SCENE_RENDERER_COMMAND_h
#define RENDER_DEBUG_LAYER_SCENE_RENDERER_COMMAND_h

#include "SceneRendererCommand.h"

class CRenderableObjectsManager;
class CXMLTreeNode;
class CRenderManager;

class CRenderDebugLayerSceneRendererCommand : public CSceneRendererCommand
{
private:
CRenderableObjectsManager *m_Layer;
public:
CRenderDebugLayerSceneRendererCommand(CXMLTreeNode &TreeNode);
virtual ~CRenderDebugLayerSceneRendererCommand();
void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_DEBUG_LAYER_SCENE_RENDERER_COMMAND_h