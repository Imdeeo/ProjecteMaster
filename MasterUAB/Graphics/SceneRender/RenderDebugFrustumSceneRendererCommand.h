#ifndef RENDER_DEBUG_FRUSTUM_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_FRUSTUM_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CXMLTreeNode;
class CRenderManager;

class CRenderDebugFrustumSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugFrustumSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CRenderDebugFrustumSceneRendererCommand(){}
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_DEBUG_FRUSTUM_SCENE_RENDERER_COMMAND_H