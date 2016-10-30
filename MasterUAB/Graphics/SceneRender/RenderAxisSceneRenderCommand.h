#ifndef RENDER_AXIS_SCENE_RENDERER_COMMAND_H
#define RENDER_AXIS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CLevel;

class CRenderAxisSceneRenderCommand :
	public CSceneRendererCommand
{
public:
	CRenderAxisSceneRenderCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CRenderAxisSceneRenderCommand();

	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_AXIS_SCENE_RENDERER_COMMAND_H