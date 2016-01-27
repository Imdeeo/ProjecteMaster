#include "RenderableObjectTechniqueManager.h"
#include "XML\XMLTreeNode.h"
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
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["renderable_object_techniques"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);

				if (l_Element.GetName() == "pool_renderable_object_technique")
				{
					m_PoolRenderableObjectTechniques.AddResource(l_Element.GetPszProperty("name"), new CPoolRenderableObjectTechnique(l_Element));
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