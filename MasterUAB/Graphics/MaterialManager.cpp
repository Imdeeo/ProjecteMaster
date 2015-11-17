#include "MaterialManager.h"

#include "XML\XMLTreeNode.h"


static CContextManager::ERasterizedState ParseRasterizerState(const std::string& str)
{
	if (str == "WIREFRAME")
	{
		return CContextManager::RS_WIREFRAME;
	}
	else if (str == "SOLID")
	{
		return CContextManager::RS_SOLID_NO_CULL;
	}
	else
	{
		return (CContextManager::ERasterizedState)-1;
	}
};

static CContextManager::EDepthStencilStates ParseDepthStencilState(const std::string& str)
{
	if (str == "DEPTH_ON")
	{
		return CContextManager::DSS_DEPTH_ON;
	}
	else if (str == "OFF")
	{
		return CContextManager::DSS_OFF;
	}
	else
	{
		return (CContextManager::EDepthStencilStates) - 1;
	}
};

static CContextManager::EBlendStates ParseBlendState(const std::string& str)
{
	if (str == "SOLID")
	{
		return CContextManager::BLEND_SOLID;
	}
	else if (str == "CLASSIC")
	{
		return CContextManager::BLEND_CLASSIC;
	}
	else if (str == "PREMULT")
	{
		return CContextManager::BLEND_PREMULT;
	}
	else
	{
		return (CContextManager::EBlendStates) - 1;
	}
};

void CMaterialManager::ClearMaterials()
{
	m_Materials.clear();
}

void CMaterialManager::AddMaterials(const std::string& path)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(path.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["materials"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Material = l_Input(i);

				std::string name = l_Material.GetPszProperty("name");

				std::unordered_map<std::string, CMaterial*>::const_iterator it = m_Materials.find(name);
				if (it != m_Materials.end()) delete it->second;

				CContextManager::ERasterizedState l_RasterizerState = ParseRasterizerState(l_Material.GetPszProperty("rasterizer_state"));
				CContextManager::EDepthStencilStates l_DepthStencilState = ParseDepthStencilState(l_Material.GetPszProperty("depth_stencil_state"));
				CContextManager::EBlendStates l_BlendState = ParseBlendState(l_Material.GetPszProperty("blend_state"));

				CMaterial *material = new CMaterial(l_RasterizerState, l_DepthStencilState, l_BlendState);

				// TODO: parse debug size & debug color
				material->SetDebugSize(5);

				m_Materials[name] = material;
			}
		}
	}
}



CMaterialManager::~CMaterialManager()
{
	for (std::unordered_map<std::string, CMaterial*>::iterator it = m_Materials.begin(); it != m_Materials.end(); ++it)
	{
		delete it->second;
	}
}
