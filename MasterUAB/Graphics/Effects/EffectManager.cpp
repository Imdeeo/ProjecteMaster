#include "Effects\EffectManager.h"
#include "XML\XMLTreeNode.h"


CEffectManager::CEffectManager(void)
{
}


CEffectManager::~CEffectManager(void)
{
}

void CEffectManager::Reload()
{
	Load(m_Filename);
}

CEffectParameters CEffectManager::m_Parameters;

bool CEffectManager::Load(const std::string &Filename)

{
	m_Filename = Filename;
	std::string l_EffectName;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["effects"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("vertex_shader"))
				{
					l_EffectName = l_Element.GetPszProperty("name");
					CEffectVertexShader *l_EffectVertexShader = new CEffectVertexShader(l_Element);
					l_EffectVertexShader->Load();
					m_VertexShaders.AddResource(l_EffectName, l_EffectVertexShader);
				} else if (l_Element.GetName() == std::string("pixel_shader"))
				{
					l_EffectName = l_Element.GetPszProperty("name");
					CEffectPixelShader *l_EffectPixelShader = new CEffectPixelShader(l_Element);
					l_EffectPixelShader->Load();
					m_PixelShaders.AddResource(l_EffectName, l_EffectPixelShader);
				} else if (l_Element.GetName() == std::string("effect_technique"))
				{
					l_EffectName = l_Element.GetPszProperty("name");
					CEffectTechnique *l_EffectTechnique = new CEffectTechnique(l_Element);
					AddResource(l_EffectName, l_EffectTechnique);
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;

}
CEffectVertexShader * CEffectManager::GetVertexShader(const std::string &VertexShader)
{
	return m_VertexShaders[VertexShader];
}
CEffectPixelShader * CEffectManager::GetPixelShader(const std::string &PixelShader)
{
	return m_PixelShaders[PixelShader];
}
