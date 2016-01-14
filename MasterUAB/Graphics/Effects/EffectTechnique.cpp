#include "Effects\EffectTechnique.h"
#include "Engine\UABEngine.h"
#include "XML\XMLTreeNode.h"

CEffectTechnique::CEffectTechnique(CXMLTreeNode &TreeNode):CNamed(TreeNode)
{
	m_VertexShaderName = TreeNode.GetPszProperty("vertex_shader");
	m_PixelShaderName = TreeNode.GetPszProperty("pixel_shader");
	m_VertexShader = UABEngine.GetEffectManager()->GetVertexShader(m_VertexShaderName);
	m_PixelShader = UABEngine.GetEffectManager()->GetPixelShader(m_PixelShaderName);
}

CEffectTechnique::~CEffectTechnique()
{

}

void CEffectTechnique::Refresh()
{
	m_VertexShader = UABEngine.GetEffectManager()->GetVertexShader(m_VertexShaderName);
	m_PixelShader = UABEngine.GetEffectManager()->GetPixelShader(m_PixelShaderName);
}

void CEffectTechnique::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	m_VertexShader->SetConstantBuffer(IdBuffer,ConstantBuffer);
	m_PixelShader->SetConstantBuffer(IdBuffer,ConstantBuffer);
}