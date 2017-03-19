#ifndef RENDER_LAYER_SCENE_RENDERER_COMMAND_H
#define RENDER_LAYER_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "LevelManager\LevelManager.h"
#include <vector>

class CRenderManager;
class CRenderableObjectsManager;
class CLevel;

class CRenderLayerSceneRendererCommand : public CSceneRendererCommand
{
private:
	std::vector<TLevelLayers*> *m_Layer;
public:
	CRenderLayerSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CRenderLayerSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_LAYER_SCENE_RENDERER_COMMAND_H