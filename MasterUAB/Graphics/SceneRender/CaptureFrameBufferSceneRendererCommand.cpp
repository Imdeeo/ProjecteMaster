#include "CaptureFrameBufferSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"
#include "Texture\CapturedFrameBufferTexture.h"
#include "Engine\UABEngine.h"
CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CStagedTexturedSceneRendererCommand(TreeNode,_LevelId)
{

}

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	m_CapturedFrameBufferTexture->Capture(0);
}
