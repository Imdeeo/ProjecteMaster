#ifndef RENDER_DEBUG_FRUSTUM_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_FRUSTUM_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CXMLTreeNode;
class CRenderManager;

class CRenderDebugFrustumSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugFrustumSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CRenderDebugFrustumSceneRendererCommand(){}
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_DEBUG_FRUSTUM_SCENE_RENDERER_COMMAND_H