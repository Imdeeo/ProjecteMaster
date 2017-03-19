#ifndef RENDER_GRID_SCENE_RENDERER_COMMAND_H
#define RENDER_GRID_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CLevel;

class CRenderGridSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CRenderGridSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel *_Level);
	virtual ~CRenderGridSceneRendererCommand();

	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_GRID_SCENE_RENDERER_COMMAND_H