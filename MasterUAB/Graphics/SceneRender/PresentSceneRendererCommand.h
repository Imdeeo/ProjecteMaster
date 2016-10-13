#ifndef PRESENT_SCENE_RENDERER_COMMAND_H
#define PRESENT_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;

class CPresentSceneRendererCommand : public CSceneRendererCommand
{
public:
	CPresentSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CPresentSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //PRESENT_SCENE_RENDERER_COMMAND_H