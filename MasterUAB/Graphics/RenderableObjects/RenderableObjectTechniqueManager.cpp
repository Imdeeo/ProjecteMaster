#include "RenderableObjectTechniqueManager.h"
#include "PoolRenderableObjectTechnique.h"
#include "Engine\UABEngine.h"
#include "Effects\EffectManager.h"
#include "PoolRenderableObjectTechnique.h"


CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager(){}

CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{
	Destroy();
}

bool CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(CPoolRenderableObjectTechnique *PoolRenderableObjectTechniques, 
	const std::string &RenderableObjectTechniqueName, const std::string &TechniqueName)
{
	return true;
}

void CRenderableObjectTechniqueManager::Destroy()
{
	m_PoolRenderableObjectTechniques.Destroy();
	CTemplatedMapManager::Destroy();
}

bool CRenderableObjectTechniqueManager::Load(const std::string &FileName)
{
	m_Filename = FileName;
	CPoolRenderableObjectTechnique* auxPoolRenderableObjectTechnique; 
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(FileName.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;
	tinyxml2::XMLElement* l_ElementAux2;

	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("renderable_object_techniques");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux != NULL)
			{
				if (l_ElementAux->Name() == std::string("pool_renderable_object_technique"))
				{
					auxPoolRenderableObjectTechnique = new  CPoolRenderableObjectTechnique(l_ElementAux);
					l_ElementAux2 = l_ElementAux->FirstChildElement();
					while (l_ElementAux2 != NULL)
					{
						if (l_ElementAux2->Name() == std::string("default_technique"))
						{
							std::string l_name = l_ElementAux2->GetPszProperty("vertex_type");
							std::string l_technique = l_ElementAux2->GetPszProperty("technique");
							AddResource(l_name, new CRenderableObjectTechnique(l_name, UABEngine.GetEffectManager()->GetResource(l_technique)));
							auxPoolRenderableObjectTechnique->AddElement(l_name, l_technique, GetResource(l_name));
						}
						if (l_ElementAux2->Name() == std::string("renderable_object_technique"))
						{
							std::string l_name = l_ElementAux2->GetPszProperty("name");
							std::string l_technique = l_ElementAux2->GetPszProperty("technique");
							AddResource(l_name, new CRenderableObjectTechnique(l_name, UABEngine.GetEffectManager()->GetResource(l_technique)));
							auxPoolRenderableObjectTechnique->AddElement(l_name, l_technique, GetResource(l_name));
						}
						l_ElementAux2 = l_ElementAux2->NextSiblingElement();
					}
					m_PoolRenderableObjectTechniques.AddResource(l_ElementAux->GetPszProperty("name"), auxPoolRenderableObjectTechnique);
				}
				l_ElementAux = l_ElementAux->NextSiblingElement();
			}
		}
	}	
	return true;
}

bool CRenderableObjectTechniqueManager::Reload()
{
	Destroy();
	return Load(m_Filename);
}