#ifndef LIGHTS_SCENE_RENDERER_COMMAND_H
#define LIGHTS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CLightsSceneRendererCommand :	public CSceneRendererCommand
{
public:
	CLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CLightsSceneRendererCommand(){}

	void Execute(CRenderManager &_RenderManager);
};

#endif //LIGHTS_SCENE_RENDERER_COMMAND_H