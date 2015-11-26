#include "Material.h"
#include "XML\XMLTreeNode.h"
#include "MiPutoEngine.h"

CMaterial::CMaterial(const CXMLTreeNode &TreeNode):CNamed("")
{
	SetName(TreeNode.GetPszProperty("name"));
	std::string effectTechnique = TreeNode.GetPszProperty("effect_technique");
	CXMLTreeNode material = TreeNode;
	for (int i = 0; i < material.GetNumChildren(); ++i)
	{
		CXMLTreeNode texture = TreeNode(i);
		if (texture.GetName() == std::string("texture"))
		{
			m_Textures.push_back(MiPutoEngine::GetInstance()->GetTextureManager()->GetTexture(TreeNode.GetPszProperty("filename")));
		}
	}
}