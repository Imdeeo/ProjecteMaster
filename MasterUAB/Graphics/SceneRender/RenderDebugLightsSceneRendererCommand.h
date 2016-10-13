#ifndef RENDER_DEBUG_LIGHT_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_LIGHT_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CXMLTreeNode;
class CRenderManager;

class CRenderDebugLightsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugLightsSceneRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CRenderDebugLightsSceneRendererCommand(){}
	virtual void Execute(CRenderManager &RenderManager);
private:
	bool m_Debug;
};

#endif //RENDER_DEBUG_LIGHT_SCENE_RENDERER_COMMAND_H