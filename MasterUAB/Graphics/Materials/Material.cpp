#include "Material.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

#include "TemplatedMaterialParameter.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode) : CNamed(TreeNode), m_CurrentParameterData(0)
{
	std::string l_RenderableObjectTechnique = TreeNode.GetPszProperty("renderable_object_technique","");
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource(l_RenderableObjectTechnique);
	CXMLTreeNode material = TreeNode;
	CEffectManager::m_RawDataCount = 0;
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
				float Value = l_Child.GetFloatProperty("value");
				m_Parameters.push_back(new CTemplatedMaterialParameter<float>(this, l_Child, Value, l_type));
			}
			if (l_type == CMaterialParameter::VECT2F)
			{
				Vect2f Value = l_Child.GetVect2fProperty("value",Vect2f(1.0f,1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect2f>(this, l_Child, Value, l_type));
			}
			if (l_type == CMaterialParameter::VECT3F)
			{
				Vect3f Value = l_Child.GetVect3fProperty("value",Vect3f(1.0f,1.0f,1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect3f>(this, l_Child, Value, l_type));
			}
			if (l_type == CMaterialParameter::VECT4F)
			{
				Vect4f Value = l_Child.GetVect4fProperty("value",Vect4f(1.0f,1.0f,1.0f,1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect4f>(this, l_Child, Value, l_type));
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
	for (int i = 0; i < m_Parameters.size(); i++)
	{
		m_Parameters[i]->Apply();
	}
}