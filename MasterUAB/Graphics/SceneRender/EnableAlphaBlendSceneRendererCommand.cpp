#include "EnableAlphaBlendSceneRendererCommand.h"

#include "RenderManager\RenderManager.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode),
	m_SrcBlend(TreeNode, "src"),
	m_DestBlend(TreeNode, "dest"),
	m_SrcAlphaBlend(TreeNode, "src_alpha"),					//C2614 illegal initialization
	m_DestAlphaBlend(TreeNode, "dest_alpha"),
	m_OpBlend(TreeNode, "operation"),
	m_OpAlphaBlend(TreeNode, "operation_alpha")
{
	m_SrcBlend = CBlend(TreeNode, "src");
	m_DestBlend = CBlend(TreeNode, "dest");
	m_SrcAlphaBlend = CBlend(TreeNode, "src_alpha");		// C2065 identificador no declarado
	m_DestAlphaBlend = CBlend(TreeNode, "dest_alpha");
	m_OpBlend = CBlendOp(TreeNode, "operation");
	m_OpAlphaBlend = CBlendOp(TreeNode, "operation_alpha");
}

CEnableAlphaBlendSceneRendererCommand::~CEnableAlphaBlendSceneRendererCommand()
{
}

void CEnableAlphaBlendSceneRendererCommand::Execute(CRenderManager &_RenderManager)
{
	//_RenderManager.EnableBlendState(&m_Blend);
}


CBlend::CBlend(const CXMLTreeNode &TreeNode, const std::string _name):
	CNamed(_name),
	m_Blend(D3D11_BLEND_ONE)
{
	const char *charName = _name.c_str();
	m_Blend = CheckBlendType(TreeNode.GetPszProperty(charName));
}

void CBlend::Destroy()
{
	// CHECKED_RELEASE(m_Blend); CHECKED_DESTROY(m_Blend); OJOCUIDAO
}

int CBlend::CheckBlendType(const std::string _type)
{
	if (_type == "zero"){ return D3D11_BLEND_ZERO; }
	if (_type == "one"){ return D3D11_BLEND_ONE; }
	if (_type == "src_color"){ return D3D11_BLEND_SRC_COLOR; }
	if (_type == "inv_src_color"){ return D3D11_BLEND_SRC_ALPHA; }
	if (_type == "src_alpha"){ return D3D11_BLEND_INV_SRC_COLOR; }
	if (_type == "inv_src_alpha"){ return D3D11_BLEND_INV_SRC_ALPHA; }
	if (_type == "dest_alpha"){ return D3D11_BLEND_DEST_ALPHA; }
	if (_type == "inv_dest_alpha"){ return D3D11_BLEND_INV_DEST_ALPHA; }
	if (_type == "dest_color"){ return D3D11_BLEND_DEST_COLOR; }
	if (_type == "inv_dest_color"){ return D3D11_BLEND_INV_DEST_COLOR; }
	if (_type == "src_alpha_sat"){ return D3D11_BLEND_SRC_ALPHA_SAT; }
	if (_type == "blend_factor"){ return D3D11_BLEND_BLEND_FACTOR; }
	if (_type == "inv_blend_factor"){ return D3D11_BLEND_INV_BLEND_FACTOR; }
	if (_type == "src1_color"){ return D3D11_BLEND_SRC1_COLOR; }
	if (_type == "inv_src1_color"){ return D3D11_BLEND_INV_SRC1_COLOR; }
	if (_type == "src1_alpha"){ return D3D11_BLEND_SRC1_ALPHA; }
	if (_type == "inv_src1_alpha"){ return D3D11_BLEND_INV_SRC1_ALPHA; }
}


CBlendOp::CBlendOp(const CXMLTreeNode &TreeNode, const std::string _name):
	CNamed(_name),
	m_BlendOp(D3D11_BLEND_OP_ADD)
{
	const char *charName = _name.c_str();
	m_BlendOp = CheckBlendOpType(TreeNode.GetPszProperty(charName));
}

void CBlendOp::Destroy()
{
	// CHECKED_RELEASE(m_BlendOp); CHECKED_DESTROY(m_BlendOp); OJOCUIDAO
}

int CBlendOp::CheckBlendOpType(const std::string _type)
{
	if (_type == "add"){ return D3D11_BLEND_OP_ADD; }
	if (_type == "substract"){ return D3D11_BLEND_OP_SUBTRACT; }
	if (_type == "rev_substract"){ return D3D11_BLEND_OP_REV_SUBTRACT; }
	if (_type == "min"){ return D3D11_BLEND_OP_MIN; }
	if (_type == "max"){ return D3D11_BLEND_OP_MAX; }
}