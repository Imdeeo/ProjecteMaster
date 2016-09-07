#ifndef DISABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H
#define DISABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CDisableAlphaBlendSceneRendererCommand :
	public CSceneRendererCommand
{

public:
	CDisableAlphaBlendSceneRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CDisableAlphaBlendSceneRendererCommand();
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //DISABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H