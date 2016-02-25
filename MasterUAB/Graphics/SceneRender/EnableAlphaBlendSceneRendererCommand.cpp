#include "EnableAlphaBlendSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
	m_Blend[0] = CheckBlendType(TreeNode.GetPszProperty("operation"));
	m_Blend[1] = CheckBlendType(TreeNode.GetPszProperty("src"));
	m_Blend[2] = CheckBlendType(TreeNode.GetPszProperty("dest"));
	m_Blend[3] = CheckBlendType(TreeNode.GetPszProperty("operation_alpha"));
	m_Blend[4] = CheckBlendType(TreeNode.GetPszProperty("src_alpha"));
	m_Blend[5] = CheckBlendType(TreeNode.GetPszProperty("dest_alpha"));

	change the enable alpha blend command to enable any blend type instead of just standard
use the ccamera::GetPositionInScreenCoordinates to calculate the directionallight in-screen position to use it as
center for the light scattering

<enable_alpha_blend
	operation="additive"
	src="one"
	dest="one"
	operation_alpha="additive"
	src_alpha="one"
	dest_alpha="one_minus_alpha"
/>

ZERO              = 1
ONE               = 2
SRC_COLOR         = 3
INV_SRC_COLOR     = 4
SRC_ALPHA         = 5
INV_SRC_ALPHA     = 6
DEST_ALPHA        = 7
INV_DEST_ALPHA    = 8
DEST_COLOR        = 9
INV_DEST_COLOR    = 10
SRC_ALPHA_SAT     = 11
BLEND_FACTOR      = 14
INV_BLEND_FACTOR  = 15
SRC1_COLOR        = 16
INV_SRC1_COLOR    = 17
SRC1_ALPHA        = 18
INV_SRC1_ALPHA    = 19
}


CEnableAlphaBlendSceneRendererCommand::~CEnableAlphaBlendSceneRendererCommand()
{
}

void CEnableAlphaBlendSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	_RenderManager.EnableBlendState(&m_Blend);
}

D3D11_BLEND CEnableAlphaBlendSceneRendererCommand::CheckBlendType(std::string _type)
{	
	if(_type == "zero"){ return D3D11_BLEND_ZERO; }
	if(_type == "one"){ return D3D11_BLEND_ONE; }
	if(_type == "src_color"){ return D3D11_BLEND_SRC_COLOR; }
	if(_type == "inv_src_color"){ return D3D11_BLEND_SRC_ALPHA; }
	if(_type == "src_alpha"){ return D3D11_BLEND_INV_SRC_COLOR; }
	if(_type == "inv_src_alpha"){ return D3D11_BLEND_INV_SRC_ALPHA; }
	if(_type == "dest_alpha"){ return D3D11_BLEND_DEST_ALPHA; }
	if(_type == "inv_dest_alpha"){ return D3D11_BLEND_INV_DEST_ALPHA; }
	if(_type == "dest_color"){ return D3D11_BLEND_DEST_COLOR; }
	if(_type == "inv_dest_color"){ return D3D11_BLEND_INV_DEST_COLOR; }
	if(_type == "src_alpha_sat"){ return D3D11_BLEND_SRC_ALPHA_SAT; }
	if(_type == "blend_factor"){ return D3D11_BLEND_BLEND_FACTOR; }
	if(_type == "inv_blend_factor"){ return D3D11_BLEND_INV_BLEND_FACTOR; }
	if(_type == "src1_color"){ return D3D11_BLEND_SRC1_COLOR; }
	if(_type == "inv_src1_color"){ return D3D11_BLEND_INV_SRC1_COLOR; }
	if(_type == "src1_alpha"){ return D3D11_BLEND_SRC1_ALPHA; }
	if(_type == "inv_src1_alpha"){ return D3D11_BLEND_INV_SRC1_ALPHA; }
}