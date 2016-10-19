#include "LightManager.h"
#include "Light.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "RenderManager\RenderManager.h"
#include "XML\tinyxml2.h"
#include "Engine\UABEngine.h"
#include "LevelManager\LevelManager.h"

CLightManager::CLightManager():m_AmbientLight(Vect4f(0.1f,0.1f,0.1f,1.0f)),m_RenderLights(false){}

CLightManager::~CLightManager(){}

bool CLightManager::Load(const std::string &FileName, const std::string &_LevelId){
	m_FileName = FileName;
	
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(FileName.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("lights");
		if (l_Element != NULL)
		{
			m_AmbientLight = l_Element->GetVect4fProperty("ambient_light_color", Vect4f(0.1f, 0.1f, 0.1f, 1.0f));
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux!=NULL)
			{				
				if (l_ElementAux->Name() == std::string("light"))
				{
					CLight::TLightType type = CLight::GetLightTypeByName(l_ElementAux->GetPszProperty("type"));
					switch(type)
					{
					case CLight::LIGHT_TYPE_OMNI:
						AddResource(l_ElementAux->GetPszProperty("name"), new COmniLight(l_ElementAux,_LevelId),_LevelId);
						break;
					case CLight::LIGHT_TYPE_DIRECTIONAL:
						AddResource(l_ElementAux->GetPszProperty("name"), new CDirectionalLight(l_ElementAux,_LevelId),_LevelId);
						break;
					case CLight::LIGHT_TYPE_SPOT:
						AddResource(l_ElementAux->GetPszProperty("name"), new CSpotLight(l_ElementAux,_LevelId),_LevelId);
						break;
					default:
						return false;
					}
				}
				l_ElementAux = l_ElementAux->NextSiblingElement();
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
	return Load(m_FileName, UABEngine.GetLevelManager()->GetActualLevel());
}

bool CLightManager::CreateNewLight(std::string _name, std::string _type , const std::string &_LevelId)
{
	CLight::TLightType type = CLight::GetLightTypeByName(_type);
	switch (type)
	{
	case CLight::LIGHT_TYPE_OMNI:
		AddResource(_name, new COmniLight(_name, _LevelId), _LevelId);
		return true;
	case CLight::LIGHT_TYPE_DIRECTIONAL:
		AddResource(_name, new CDirectionalLight(_name, _LevelId), _LevelId);
		return true;
	case CLight::LIGHT_TYPE_SPOT:
		AddResource(_name, new CSpotLight(_name, _LevelId), _LevelId);
		return true;
	default:
		return false;
	}
}

bool CLightManager::RenderAux(CRenderManager *_RenderManager){
	if (m_RenderLights)
	{
		Render(_RenderManager);
	}
	return true;
}

bool CLightManager::Render(CRenderManager *_RenderManager){
	for (size_t i = 0; i < GetResourcesVector().size(); i++)
	{
		GetResourcesVector()[i]->Render(_RenderManager);
	}	
	return true;
}

void CLightManager::Save()
{
	FILE* l_File;
	if (!fopen_s(&l_File, m_FileName.c_str(), "w"))
	{
		fprintf_s(l_File, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
		fprintf_s(l_File, "<lights ambient_light_color=\"%f %f %f %f\">\n", 
			m_AmbientLight.x, m_AmbientLight.y, m_AmbientLight.z, m_AmbientLight.w);

		typedef TMapResources::iterator it_type;		
		for (it_type iterator = m_ResourcesMap.begin(); iterator != m_ResourcesMap.end(); iterator++)
		{
			iterator->second.m_Value->Save(l_File);
		}

		fprintf_s(l_File, "</lights>\n");
		fclose(l_File);
	}
}