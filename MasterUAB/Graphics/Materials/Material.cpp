#include "Material.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Effects\EffectManager.h"
#include "Texture\TextureManager.h"

#include "RenderableObjects\RenderableObjectTechnique.h"
#include "TemplatedMaterialParameter.h"
#include "Texture\DynamicTexture.h"
#include "VideoManager\VideoManager.h"
#include "theoraplayer\TheoraPlayer.h"
#include "D3D11.h"
#include "RenderManager\RenderManager.h"

#define INDEX_DIFFUSE_TEXTURE 0
#define INDEX_LIGHTMAP_TEXTURE 1
#define INDEX_NORMAL_TEXTURE 2
#define INDEX_LIGHTMAP_2_TEXTURE 3
#define INDEX_LIGHTMAP_3_TEXTURE 4
#define INDEX_GLOSSINESS_TEXTURE 5
#define INDEX_CUBEMAP_TEXTURE 8
#define INDEX_SPECULAR_TEXTURE 10

CMaterial::CMaterial(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CNamed(TreeNode), CLevelInfo(_LevelId), m_CurrentParameterData(0)
{
	std::string l_RenderableObjectTechnique = TreeNode->GetPszProperty("renderable_object_technique","");
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource(l_RenderableObjectTechnique);

	tinyxml2::XMLElement* l_Element = TreeNode->FirstChildElement();
	CEffectManager::m_RawDataCount = 0;
	for (size_t i = 0; i < MAX_TEXTURES; i++)
	{
		m_Textures[i] = nullptr;
	}
	while (l_Element != NULL)
	{
		if (l_Element->Name() == std::string("texture"))
		{
			std::string l_TextureType = l_Element->GetPszProperty("type", "diffuse");
			std::string l_FileName = l_Element->GetPszProperty("filename");
			if (l_TextureType == "video")
			{
				IVideoManager* l_VideoManager = UABEngine.GetVideoManager(); 
				TheoraVideoClip* l_Clip = l_VideoManager->LoadVideoClip(l_FileName, l_Element->GetBoolProperty("loop", false));
				CDynamicTexture* l_DynamicTexture = new CDynamicTexture(l_FileName, l_Clip->getWidth(), l_Clip->getHeight(), false, "r8u");
				l_DynamicTexture->SetType(l_TextureType);
				m_Textures[INDEX_DIFFUSE_TEXTURE] = l_DynamicTexture;
			}
			else
			{
				CTexture* l_texture = CUABEngine::GetInstance()->GetTextureManager()->GetTexture(l_FileName,_LevelId);

				l_texture->SetType(l_TextureType);

				if (l_TextureType == "lightmap")
				{
					m_Textures[INDEX_LIGHTMAP_TEXTURE] = l_texture;
				}
				if (l_TextureType == "lightmap2")
				{
					m_Textures[INDEX_LIGHTMAP_2_TEXTURE] = l_texture;
				}
				if (l_TextureType == "lightmap3")
				{
					m_Textures[INDEX_LIGHTMAP_3_TEXTURE] = l_texture;
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
				if (l_TextureType == "glossiness")
				{
					m_Textures[INDEX_GLOSSINESS_TEXTURE] = l_texture;
				}
				if (l_TextureType == "specular")
				{
					m_Textures[INDEX_SPECULAR_TEXTURE] = l_texture;
				}
			}			
		}
		if (l_Element->Name() == std::string("parameter"))
		{
			CMaterialParameter::TMaterialType l_type = CMaterialParameter::GetTypeFromString(l_Element->GetPszProperty("type"));
			std::string l_Description;
			const char * l_existDescription = l_Element->GetPszProperty("description");
			if (l_existDescription == NULL)
			{
				l_Description = "";
			}
			else
			{
				l_Description = l_Element->GetPszProperty("description");
			}
			if (l_type == CMaterialParameter::FLOAT)
			{
				float Value = l_Element->GetFloatProperty("value");
				
				m_Parameters.push_back(new CTemplatedMaterialParameter<float>(this, l_Element, Value, l_type, l_Description));
			}
			if (l_type == CMaterialParameter::VECT2F)
			{
				Vect2f Value = l_Element->GetVect2fProperty("value", Vect2f(1.0f, 1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect2f>(this, l_Element, Value, l_type, l_Description));
			}
			if (l_type == CMaterialParameter::VECT3F)
			{
				Vect3f Value = l_Element->GetVect3fProperty("value", Vect3f(1.0f, 1.0f, 1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect3f>(this, l_Element, Value, l_type, l_Description));
			}
			if (l_type == CMaterialParameter::VECT4F)
			{
				Vect4f Value = l_Element->GetVect4fProperty("value", Vect4f(1.0f, 1.0f, 1.0f, 1.0f));
				m_Parameters.push_back(new CTemplatedMaterialParameter<Vect4f>(this, l_Element, Value, l_type, l_Description));
			}
			if (l_type == CMaterialParameter::COLOR)
			{
				Vect4f Value = l_Element->GetVect4fProperty("value", Vect4f(1.0f, 1.0f, 1.0f, 1.0f));
				CColor l_Aux(Value.x, Value.y, Value.z, Value.w);
				m_Parameters.push_back(new CTemplatedMaterialParameter<CColor>(this, l_Element, l_Aux, l_type, l_Description));
			}
		}
		l_Element = l_Element->NextSiblingElement();
	}
}

CMaterial::~CMaterial()
{
	Destroy();
}

void CMaterial::Destroy()
{
	for (size_t i = 0; i < m_Parameters.size(); i++)
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
			if (m_Textures[i]->GetType() == "video")
			{
				IVideoManager* l_VideoManager = UABEngine.GetVideoManager();
				TheoraVideoClip* l_Clip = l_VideoManager->GetClip(m_Textures[i]->GetName());
				if (l_Clip != nullptr)
				{
					TheoraVideoFrame *l_Frame = l_Clip->getNextFrame();
					if (l_Frame)
					{
						D3D11_TEXTURE2D_DESC textureDesc;

						// Initialize the render target texture description.
						ZeroMemory(&textureDesc, sizeof(textureDesc));

						// Setup the render target texture description.
						textureDesc.Width = l_Frame->getWidth();
						textureDesc.Height = l_Frame->getHeight();
						textureDesc.MipLevels = 1;
						textureDesc.ArraySize = 1;
						textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
						textureDesc.SampleDesc.Count = 1;
						textureDesc.Usage = D3D11_USAGE_DEFAULT;
						textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
						textureDesc.CPUAccessFlags = 0;
						textureDesc.MiscFlags = 0;

						D3D11_SUBRESOURCE_DATA srd;
						srd.pSysMem = l_Frame->getBuffer();
						srd.SysMemPitch = l_Frame->getWidth() * 4;
						srd.SysMemSlicePitch = 0;


						ID3D11Texture2D* l_Texture2D = NULL;
						ID3D11Device* l_Device = UABEngine.GetRenderManager()->GetDevice();
						//HRESULT l_HR = l_Device->CreateTexture2D(&textureDesc, &srd, &l_Texture2D);
						HRESULT l_HR = l_Device->CreateTexture2D(&textureDesc, &srd, &l_Texture2D);
						if (!FAILED(l_HR))
						{
							l_HR = l_Device->CreateShaderResourceView(l_Texture2D, 0, &m_Textures[i]->m_Texture);
						}
						l_Clip->popFrame(); // be sure to pop the frame from the frame queue when you're done
					}
				}
			}
			m_Textures[i]->Activate(i);
		}
	}
	for (size_t i = 0; i < m_Parameters.size(); i++)
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
	for (size_t i = 0; i < b.GetParameters().size(); i++)
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
		if (l_type == CMaterialParameter::COLOR)
		{
			m_Parameters.push_back(new CTemplatedMaterialParameter<CColor>(*((CTemplatedMaterialParameter<CColor>*)l_MaterialParameter)));			
		}
	}
}

void CMaterial::SetValue(int _index, float _value)
{
	((CTemplatedMaterialParameter<float>*)m_Parameters[_index])->SetValue(_value);
}

float CMaterial::GetValue(int _index)
{
	return ((CTemplatedMaterialParameter<float>*)m_Parameters[_index])->GetValue();
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

CTexture * CMaterial::GetTexture(int index)
{
	return m_Textures[index];
}

void CMaterial::CopyParameters(std::vector<CMaterialParameter *>_NewParameters, bool _destroy)
{
	if (_destroy)
	{
		for (size_t i = 0; i < m_Parameters.size(); i++)
		{
			CHECKED_DELETE(m_Parameters[i]);
		}
	}
	m_Parameters.clear();

	for (size_t i = 0; i < _NewParameters.size(); i++)
	{
		CMaterialParameter* l_MaterialParameter = _NewParameters[i];
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
		if (l_type == CMaterialParameter::COLOR)
		{
			m_Parameters.push_back(new CTemplatedMaterialParameter<CColor>(*((CTemplatedMaterialParameter<CColor>*)l_MaterialParameter)));
		}
	}
}