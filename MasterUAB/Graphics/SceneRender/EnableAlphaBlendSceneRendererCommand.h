#ifndef ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H
#define ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CEnableAlphaBlendSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CEnableAlphaBlendSceneRendererCommand();

	virtual void Execute(CRenderManager &RenderManager);
};

#endif //ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H