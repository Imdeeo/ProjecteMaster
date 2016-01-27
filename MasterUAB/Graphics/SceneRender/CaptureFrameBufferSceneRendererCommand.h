#ifndef CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H
#define CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CCaptureFrameBufferSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CCaptureFrameBufferSceneRendererCommand();

	void Execute(CRenderManager &_RenderManager);
};

#endif //CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H