#ifndef RENDER_GRID_SCENE_RENDERER_COMMAND_H
#define RENDER_GRID_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CRenderGridSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CRenderGridSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CRenderGridSceneRendererCommand();

	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_GRID_SCENE_RENDERER_COMMAND_H