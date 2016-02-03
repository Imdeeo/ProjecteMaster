#ifndef SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#include "StagedTexturedSceneRendererCommand.h"

class CSetRenderTargetSceneRendererCommand :
	public CStagedTexturedSceneRendererCommand
{
public:
	CSetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CSetRenderTargetSceneRendererCommand();

	void Execute(CRenderManager &_RenderManager);
};

#endif //SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
