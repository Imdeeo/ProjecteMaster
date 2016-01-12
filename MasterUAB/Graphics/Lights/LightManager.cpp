#include "LightManager.h"
#include "Light.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "RenderManager\RenderManager.h"

#include "XML\XMLTreeNode.h"

CLightManager::CLightManager(){}

CLightManager::~CLightManager(){}

bool CLightManager::Load(const std::string &FileName){
	m_FileName = FileName;
	
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["lights"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{				
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("light"))
				{
					CLight::TLightType type = CLight::GetLightTypeByName(l_Element.GetPszProperty("type"));
					switch(type)
					{
					case CLight::LIGHT_TYPE_OMNI:
						AddResource(l_Element.GetPszProperty("name"), new COmniLight(l_Element));
						break;
					case CLight::LIGHT_TYPE_DIRECTIONAL:
						AddResource(l_Element.GetPszProperty("name"), new CDirectionalLight(l_Element));
						break;
					case CLight::LIGHT_TYPE_SPOT:
						AddResource(l_Element.GetPszProperty("name"), new CSpotLight(l_Element));
						break;
					default:
						return false;
					}
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

bool CLightManager::Reload(){
	Destroy();
	return Load(m_FileName);
}

bool CLightManager::Render(CRenderManager *RenderManager){
	return true;
}