#include "LightManager.h"
#include "Light.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "RenderManager\RenderManager.h"

#include "XML\XMLTreeNode.h"

CLightManager::CLightManager():m_AmbientLight(Vect4f(0.1f,0.1f,0.1f,1.0f)),m_RenderLights(false){}

CLightManager::~CLightManager(){}

bool CLightManager::Load(const std::string &FileName){
	m_FileName = FileName;
	
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["lights"];
		if (l_Input.Exists())
		{
			m_AmbientLight = l_Input.GetVect4fProperty("ambient_light_color", Vect4f(0.1f, 0.1f, 0.1f, 1.0f));
			m_FogColor = l_Input.GetVect3fProperty("fog_color", Vect3f(1.0f, 1.0f, 1.0f));
			m_FogMaxAttenuation = l_Input.GetFloatProperty("fog_max_attenuation");
			m_FogStart = l_Input.GetFloatProperty("fog_start");
			m_FogEnd = l_Input.GetFloatProperty("fog_end");
			m_FogEnabled= l_Input.GetBoolProperty("fog_enabled");
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

#ifdef _DEBUG
bool CLightManager::Render(CRenderManager *_RenderManager){
	if (m_RenderLights)
	{
		for (int i = 0; i < GetResourcesVector().size(); i++)
		{
			GetResourcesVector()[i]->Render(_RenderManager);
		}
	}
	return true;
}
#endif

void CLightManager::Save()
{
	FILE* l_File;
	if (!fopen_s(&l_File, m_FileName.c_str(), "w"))
	{
		fprintf_s(l_File, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
		fprintf_s(l_File, "<lights ambient_light_color=\"%f %f %f %f\" fog_color=\"%f %f %f\" fog_max_attenuation=\"%f\" fog_start=\"%f\" fog_end=\"%f\" fog_enabled=\"%s\">\n",
			m_AmbientLight.x, m_AmbientLight.y, m_AmbientLight.z, m_AmbientLight.w, m_FogColor.x, m_FogColor.y, m_FogColor.z, m_FogMaxAttenuation, m_FogStart, m_FogEnd, m_FogEnabled ? "true" : "false");

		typedef TMapResources::iterator it_type;		
		for (it_type iterator = m_ResourcesMap.begin(); iterator != m_ResourcesMap.end(); iterator++)
		{
			iterator->second.m_Value->Save(l_File);
		}

		fprintf_s(l_File, "</lights>\n");
		fclose(l_File);
	}
}