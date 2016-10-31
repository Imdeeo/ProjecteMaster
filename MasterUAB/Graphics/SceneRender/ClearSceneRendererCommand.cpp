#include "ClearSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"

#include "LevelManager\Level.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level)
{
	m_RenderTarget = TreeNode->GetBoolProperty("render_target");
	m_DepthStencil = TreeNode->GetBoolProperty("depth_stencil");
}

CClearSceneRendererCommand::~CClearSceneRendererCommand(){}

void CClearSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.Clear(m_RenderTarget, m_DepthStencil);
}