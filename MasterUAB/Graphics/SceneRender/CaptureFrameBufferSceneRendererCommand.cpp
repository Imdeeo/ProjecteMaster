#include "CaptureFrameBufferSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"
#include "Texture\CapturedFrameBufferTexture.h"
#include "Engine\UABEngine.h"
#include "LevelManager\Level.h"

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel*  _Level) : CStagedTexturedSceneRendererCommand(TreeNode,_Level)
{

}

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	m_CapturedFrameBufferTexture->Capture(0);
}
