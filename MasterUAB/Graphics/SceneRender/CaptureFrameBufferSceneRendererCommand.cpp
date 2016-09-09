#include "CaptureFrameBufferSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"
#include "Texture\CapturedFrameBufferTexture.h"
#include "Engine\UABEngine.h"
CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CStagedTexturedSceneRendererCommand(TreeNode)
{

}

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	m_CapturedFrameBufferTexture->Capture(0);
}
