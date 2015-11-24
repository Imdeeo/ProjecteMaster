#include "EffectTechnique.h"
#include "XML\XMLTreeNode.h"

CEffectTechnique::CEffectTechnique(CXMLTreeNode &TreeNode):CNamed("")
{
	SetName(TreeNode.GetPszProperty("name"));
	m_VertexShaderName = TreeNode.GetPszProperty("vertex_shader");
	m_PixelShaderName = TreeNode.GetPszProperty("pixel_shader");
}

CEffectTechnique::~CEffectTechnique()
{

}

void CEffectTechnique::Refresh()
{
	
}