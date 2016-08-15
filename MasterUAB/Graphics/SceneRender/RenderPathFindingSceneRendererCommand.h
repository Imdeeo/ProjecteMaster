#ifndef RENDER_PATHFINDING_SCENE_RENDERER_COMMAND_H
#define RENDER_PATHFINDING_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CRenderPathFindingSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CRenderPathFindingSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CRenderPathFindingSceneRendererCommand();

	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_PATHFINDING_SCENE_RENDERER_COMMAND_H