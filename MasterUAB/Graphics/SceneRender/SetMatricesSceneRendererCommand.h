#ifndef SET_MATRICES_SCENE_RENDERER_COMMAND_H
#define SET_MATRICES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;

class CSetMatricesSceneRendererCommand : public CSceneRendererCommand
{
public:
	CSetMatricesSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CSetMatricesSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //SET_MATRICES_SCENE_RENDERER_COMMAND_H