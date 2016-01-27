#ifndef RENDER_LAYER_SCENE_RENDERER_COMMAND_H
#define RENDER_LAYER_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CRenderableObjectsManager;

class CRenderLayerSceneRendererCommand : public CSceneRendererCommand
{
private:
	CRenderableObjectsManager *m_Layer;
public:
	CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CRenderLayerSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_LAYER_SCENE_RENDERER_COMMAND_H