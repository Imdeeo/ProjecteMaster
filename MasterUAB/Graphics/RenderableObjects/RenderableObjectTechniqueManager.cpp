#include "RenderableObjectTechniqueManager.h"
#include "XML\XMLTreeNode.h"
#include "PoolRenderableObjectTechnique.h"
#include "Engine\UABEngine.h"
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
	CXMLTreeNode l_XML;
	CPoolRenderableObjectTechnique* auxPoolRenderableObjectTechnique; 
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["renderable_object_techniques"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);		
				if (l_Element.GetName() == std::string("pool_renderable_object_technique"))
				{
					auxPoolRenderableObjectTechnique = new  CPoolRenderableObjectTechnique(l_Element);
					for (size_t i = 0; i < l_Element.GetNumChildren(); i++)
					{
						CXMLTreeNode l_ElementAux = l_Element(i);
						if (l_ElementAux.GetName() == std::string("default_technique"))
						{
							std::string l_name = l_ElementAux.GetPszProperty("vertex_type");
							std::string l_technique = l_ElementAux.GetPszProperty("technique");
							AddResource(l_name, new CRenderableObjectTechnique(l_name, UABEngine.GetEffectManager()->GetResource(l_technique)));
						}
						if (l_ElementAux.GetName() == std::string("renderable_object_technique"))
						{
							std::string l_name = l_ElementAux.GetPszProperty("name");
							std::string l_technique = l_ElementAux.GetPszProperty("technique");
							AddResource(l_name, new CRenderableObjectTechnique(l_name, UABEngine.GetEffectManager()->GetResource(l_technique)));
							auxPoolRenderableObjectTechnique->AddElement(l_name, l_technique, GetResource(l_name));
						}
					}
					m_PoolRenderableObjectTechniques.AddResource(l_Element.GetPszProperty("name"), auxPoolRenderableObjectTechnique);
				}
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