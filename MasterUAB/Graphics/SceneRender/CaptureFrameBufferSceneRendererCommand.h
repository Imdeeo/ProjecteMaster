#ifndef CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H
#define CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedSceneRendererCommand.h"
class CCaptureFrameBufferSceneRendererCommand :
	public CStagedTexturedSceneRendererCommand
{
public:
	CCaptureFrameBufferSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CCaptureFrameBufferSceneRendererCommand();

	void Execute(CRenderManager &_RenderManager);
};

#endif //CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H