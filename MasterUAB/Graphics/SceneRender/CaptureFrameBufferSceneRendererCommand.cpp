#include "CaptureFrameBufferSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"
#include "Texture\CapturedFrameBufferTexture.h"
#include "Engine\UABEngine.h"
CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{

}

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	m_CapturedFrameBufferTexture->Capture(0);
	UABEngine.GetTextureManager()->AddResource(m_CapturedFrameBufferTexture->GetName(),m_CapturedFrameBufferTexture);
}
