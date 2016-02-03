#include "CaptureFrameBufferSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{

}

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	//TODO
}
