#ifndef UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CUnsetRenderTargetSceneRendererCommand : public CSceneRendererCommand
{
public:
	CUnsetRenderTargetSceneRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CUnsetRenderTargetSceneRendererCommand(){};
	void Execute(CRenderManager &RenderManager);
};

#endif //UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H