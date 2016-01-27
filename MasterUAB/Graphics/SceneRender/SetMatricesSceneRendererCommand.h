#ifndef SET_MATRICES_SCENE_RENDERER_COMMAND_H
#define SET_MATRICES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;

class CSetMatricesSceneRendererCommand : public CSceneRendererCommand
{
public:
	CSetMatricesSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CSetMatricesSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //SET_MATRICES_SCENE_RENDERER_COMMAND_H