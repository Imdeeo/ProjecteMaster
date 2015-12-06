#include "Materials\Material.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CMaterial::CMaterial(const CXMLTreeNode &TreeNode):CNamed(TreeNode)
{
	std::string effectTechnique = TreeNode.GetPszProperty("effect_technique");
	m_EffectTechnique = UABEngine.GetEffectManager()->GetResource(effectTechnique);
	CXMLTreeNode material = TreeNode;
	for (int i = 0; i < material.GetNumChildren(); ++i)
	{
		CXMLTreeNode texture = TreeNode(i);
		if (texture.GetName() == std::string("texture"))
		{
			m_Textures.push_back(CUABEngine::GetInstance()->GetTextureManager()->GetTexture(texture.GetPszProperty("filename")));
		}
	}
}

void CMaterial::Apply()
{
	for(size_t i=0;i<m_Textures.size();++i)
		m_Textures[i]->Activate(i);
}
