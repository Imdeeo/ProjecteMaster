#ifndef SET_MATRICES_SCENE_RENDERER_COMMAND_H
#define SET_MATRICES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CLevel;

class CSetMatricesSceneRendererCommand : public CSceneRendererCommand
{
public:
	CSetMatricesSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel *_Level);
	virtual ~CSetMatricesSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //SET_MATRICES_SCENE_RENDERER_COMMAND_H