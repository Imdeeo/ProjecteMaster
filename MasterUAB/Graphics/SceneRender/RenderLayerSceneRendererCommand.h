#ifndef RENDER_LAYER_SCENE_RENDERER_COMMAND_H
#define RENDER_LAYER_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include <vector>

class CRenderManager;
class CRenderableObjectsManager;

class CRenderLayerSceneRendererCommand : public CSceneRendererCommand
{
private:
	std::vector<CRenderableObjectsManager*> *m_Layer;
public:
	CRenderLayerSceneRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CRenderLayerSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_LAYER_SCENE_RENDERER_COMMAND_H