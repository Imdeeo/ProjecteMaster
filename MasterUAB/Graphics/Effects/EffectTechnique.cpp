#include "Effects\EffectTechnique.h"

#include "Engine\UABEngine.h"
#include "EffectManager.h"

#include "XML\XMLTreeNode.h"

CEffectTechnique::CEffectTechnique(CXMLTreeNode &TreeNode):CNamed(TreeNode)
{
	m_VertexShaderName = TreeNode.GetPszProperty("vertex_shader");
	m_PixelShaderName = TreeNode.GetPszProperty("pixel_shader");
	m_GeometryShaderName = TreeNode.GetPszProperty("geometry_shader","");
	m_VertexShader = UABEngine.GetEffectManager()->GetVertexShader(m_VertexShaderName);
	m_PixelShader = UABEngine.GetEffectManager()->GetPixelShader(m_PixelShaderName);
	m_GeometryShader = UABEngine.GetEffectManager()->GetGeometryShader(m_GeometryShaderName);
}

CEffectTechnique::~CEffectTechnique()
{

}

void CEffectTechnique::Refresh()
{
	m_VertexShader = UABEngine.GetEffectManager()->GetVertexShader(m_VertexShaderName);
	m_PixelShader = UABEngine.GetEffectManager()->GetPixelShader(m_PixelShaderName);
	m_GeometryShader = UABEngine.GetEffectManager()->GetGeometryShader(m_GeometryShaderName);
}

void CEffectTechnique::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	m_VertexShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
	m_PixelShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
	if (m_GeometryShader != NULL)
		m_GeometryShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
}

CEffectVertexShader* CEffectTechnique::GetVertexShader()
{
	return m_VertexShader;
}

CEffectPixelShader* CEffectTechnique::GetPixelShader()
{
	return m_PixelShader;
}

CEffectGeometryShader* CEffectTechnique::GetGeometryShader()
{
	return m_GeometryShader;
}