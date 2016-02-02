#include "Material.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

#include "TemplatedMaterialParameter.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode) : CNamed(TreeNode), m_CurrentParameterData(0)
{
	std::string l_RenderableObjectTechnique = TreeNode.GetPszProperty("renderable_object_technique","");
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
			CMaterialParameter::TMaterialType l_type = CMaterialParameter::GetTypeFromString(l_Child.GetPszProperty("type"));
			if (l_type == CMaterialParameter::FLOAT)
			{
				m_Parameters.push_back(new CTemplatedMaterialParameter<float>(this, l_Child, l_type));
			}
			if (l_type == CMaterialParameter::VECT2F)
			{
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect2f>(this, l_Child, l_type));
			}
			if (l_type == CMaterialParameter::VECT3F)
			{
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect3f>(this, l_Child, l_type));
			}
			if (l_type == CMaterialParameter::VECT4F)
			{
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect4f>(this, l_Child, l_type));
			}
		}

	}
}

CMaterial::~CMaterial()
{
	Destroy();
}

void CMaterial::Destroy()
{
	for (int i = 0; i < m_Parameters.size(); i++)
	{
		CHECKED_DELETE(m_Parameters[i]);
	}
	m_Parameters.clear();
}

void * CMaterial::GetNextParameterAddress(unsigned int NumBytes)
{

	return m_Parameters[m_CurrentParameterData + 1]->GetValueAddress();
}

void CMaterial::Apply(CRenderableObjectTechnique *RenderableObjectTechnique)
{
	for (int i = 0; i < m_Textures.size(); i++)
	{
		m_Textures[i]->Activate(i);
	}
}