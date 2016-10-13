#include "SetDepthStencilStateSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include <d3d11.h>
#include "Engine\UABEngine.h"

#include "Utils.h"

CSetDepthStencilStateSceneRendererCommand::CSetDepthStencilStateSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CSceneRendererCommand(TreeNode,_LevelId)
{
	D3D11_DEPTH_STENCIL_DESC l_DepthStencilStateDescription;
	ZeroMemory(&l_DepthStencilStateDescription,sizeof(D3D11_DEPTH_STENCIL_DESC));
	l_DepthStencilStateDescription.DepthEnable=TreeNode->GetBoolProperty("enable_z_test", false) ? TRUE : FALSE;
	l_DepthStencilStateDescription.DepthWriteMask=TreeNode->GetBoolProperty("write_z_buffer", false) ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	l_DepthStencilStateDescription.DepthFunc=D3D11_COMPARISON_LESS_EQUAL;
	l_DepthStencilStateDescription.StencilEnable=TreeNode->GetBoolProperty("enable_stencil", false) ? TRUE : FALSE;
	if(FAILED(UABEngine.GetRenderManager()->GetDevice()->CreateDepthStencilState(&l_DepthStencilStateDescription, &m_DepthStencilState)))
		printf("Error on creating CSetDepthStencilStateSceneRendererCommand");
}

CSetDepthStencilStateSceneRendererCommand::~CSetDepthStencilStateSceneRendererCommand()
{
	CHECKED_RELEASE(m_DepthStencilState);
}

void CSetDepthStencilStateSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetDeviceContext()->OMSetDepthStencilState(m_DepthStencilState, 0);
}