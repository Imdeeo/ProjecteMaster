#ifndef RENDER_PATHFINDING_SCENE_RENDERER_COMMAND_H
#define RENDER_PATHFINDING_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CRenderPathFindingSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CRenderPathFindingSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CRenderPathFindingSceneRendererCommand();

	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_PATHFINDING_SCENE_RENDERER_COMMAND_H