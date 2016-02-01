#include "Material.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

#include "TemplatedMaterialParameter.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	std::string l_RenderableObjectTechnique = TreeNode.GetPszProperty("renderable_object_technique");
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource(l_RenderableObjectTechnique);
	CXMLTreeNode material = TreeNode;
	for (int i = 0; i < material.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Child = TreeNode(i);
		if (l_Child.GetName() == std::string("texture"))
		{
			m_Textures.push_back(CUABEngine::GetInstance()->GetTextureManager()->GetTexture(l_Child.GetPszProperty("filename")));
		}
		if (l_Child.GetName() == std::string("parameter"))
		{
			std::string type = l_Child.GetPszProperty("type");
			CMaterialParameter::GetTypeFromString(type);
			m_Parameters.push_back(new CTemplatedMaterialParameter<>(this,l_Child,));
		}
		if (l_Child.GetName() == std::string("texture"))
		{
			m_Textures.push_back(CUABEngine::GetInstance()->GetTextureManager()->GetTexture(l_Child.GetPszProperty("filename")));
		}

	}
}
