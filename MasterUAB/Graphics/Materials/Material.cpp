#include "Material.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Effects\EffectManager.h"
#include "Texture\TextureManager.h"

#include "RenderableObjects\RenderableObjectTechnique.h"
#include "TemplatedMaterialParameter.h"

#define INDEX_DIFFUSE_TEXTURE 0
#define INDEX_LIGHTMAP_TEXTURE 1
#define INDEX_NORMAL_TEXTURE 2
#define INDEX_CUBEMAP_TEXTURE 8

CMaterial::CMaterial(const CXMLTreeNode &TreeNode) : CNamed(TreeNode), m_CurrentParameterData(0)
{
	std::string l_RenderableObjectTechnique = TreeNode.GetPszProperty("renderable_object_technique","");
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource(l_RenderableObjectTechnique);
	CXMLTreeNode material = TreeNode;
	CEffectManager::m_RawDataCount = 0;
	for (size_t i = 0; i < MAX_TEXTURES; i++)
	{
		m_Textures[i] = nullptr;
	}
	for (int i = 0; i < material.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Child = TreeNode(i);
		if (l_Child.GetName() == std::string("texture"))
		{
			CTexture* l_texture = CUABEngine::GetInstance()->GetTextureManager()->GetTexture(l_Child.GetPszProperty("filename"));
			std::string l_TextureType = l_Child.GetPszProperty("type");
			l_texture->SetType(l_TextureType);

			if (l_TextureType == "lightmap")
			{
				m_Textures[INDEX_LIGHTMAP_TEXTURE] = l_texture;
			}
			if (l_TextureType == "normal")
			{
				m_Textures[INDEX_NORMAL_TEXTURE] = l_texture;
			}
			if (l_TextureType == "reflection")
			{
				m_Textures[INDEX_CUBEMAP_TEXTURE] = l_texture;
			}
			if (l_TextureType == "diffuse")
			{
				m_Textures[INDEX_DIFFUSE_TEXTURE] = l_texture;
			}
		}
		if (l_Child.GetName() == std::string("parameter"))
		{
			CMaterialParameter::TMaterialType l_type = CMaterialParameter::GetTypeFromString(l_Child.GetPszProperty("type"));
			std::string l_Description;
			const char * l_existDescription = l_Child.GetPszProperty("description");
			if (l_existDescription == NULL)
			{
				l_Description = "";
			}
			else
			{
				l_Description = l_Child.GetPszProperty("description");
			}
			if (l_type == CMaterialParameter::FLOAT)
			{
				float Value = l_Child.GetFloatProperty("value");
				
				m_Parameters.push_back(new CTemplatedMaterialParameter<float>(this, l_Child, Value, l_type, l_Description));
			}
			if (l_type == CMaterialParameter::VECT2F)
			{
				Vect2f Value = l_Child.GetVect2fProperty("value",Vect2f(1.0f,1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect2f>(this, l_Child, Value, l_type, l_Description));
			}
			if (l_type == CMaterialParameter::VECT3F)
			{
				Vect3f Value = l_Child.GetVect3fProperty("value",Vect3f(1.0f,1.0f,1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect3f>(this, l_Child, Value, l_type, l_Description));
			}
			if (l_type == CMaterialParameter::VECT4F)
			{
				Vect4f Value = l_Child.GetVect4fProperty("value",Vect4f(1.0f,1.0f,1.0f,1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect4f>(this, l_Child, Value, l_type, l_Description));
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
	for (int i = 0; i <MAX_TEXTURES; i++)
	{
		if (m_Textures[i] != nullptr)
		{
			m_Textures[i]->Activate(i);
		}
	}
	for (int i = 0; i < m_Parameters.size(); i++)
	{
		m_Parameters[i]->Apply();
	}
}


void CMaterial::operator=(CMaterial &b)
{
	Destroy();
	for (size_t i = 0; i < MAX_TEXTURES; i++)
	{
		m_Textures[i] = b.m_Textures[i];
	}
	m_RenderableObjectTechnique = b.m_RenderableObjectTechnique;
	m_CurrentParameterData = b.m_CurrentParameterData;
	for (int i = 0; i < b.GetParameters().size(); i++)
	{
		CMaterialParameter* l_MaterialParameter = b.GetParameters()[i];
		CMaterialParameter::TMaterialType l_type = l_MaterialParameter->getMaterialType();
		if (l_type == CMaterialParameter::FLOAT)
		{
			m_Parameters.push_back(new CTemplatedMaterialParameter<float>(*((CTemplatedMaterialParameter<float>*)l_MaterialParameter)));
		}
		if (l_type == CMaterialParameter::VECT2F)
		{
			m_Parameters.push_back(new CTemplatedMaterialParameter<Vect2f>(*((CTemplatedMaterialParameter<Vect2f>*)l_MaterialParameter)));
		}
		if (l_type == CMaterialParameter::VECT3F)
		{
			m_Parameters.push_back(new CTemplatedMaterialParameter<Vect3f>(*((CTemplatedMaterialParameter<Vect3f>*)l_MaterialParameter)));
		}
		if (l_type == CMaterialParameter::VECT4F)
		{
			m_Parameters.push_back(new CTemplatedMaterialParameter<Vect4f>(*((CTemplatedMaterialParameter<Vect4f>*)l_MaterialParameter)));
		}
	}
}

CRenderableObjectTechnique* CMaterial::GetRenderableObjectTechnique()
{
	return m_RenderableObjectTechnique;
}

void CMaterial::Save(FILE* _File)
{
	fprintf_s(_File, "\t<material name=\"%s\" renderable_object_technique=\"%s\">\n",m_Name.c_str(),m_RenderableObjectTechnique->GetName().c_str());

	for (size_t i = 0; i < MAX_TEXTURES; i++)
	{
		if (m_Textures[i]!=nullptr)
		{
			m_Textures[i]->Save(_File, 2);
		}
	}
	for (size_t i = 0; i < m_Parameters.size(); i++)
	{
		m_Parameters[i]->Save(_File, 2);
	}

	fprintf_s(_File, "\t</material>\n");
}
