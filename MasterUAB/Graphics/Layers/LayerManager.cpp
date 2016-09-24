#include "LayerManager.h"
#include "RenderManager\RenderManager.h"
#include "Utils.h"

#define DEFAULT_LAYER_INITIAL_NAME "default"

CLayerManager::CLayerManager():m_DefaultLayer(nullptr)
{
	const char * l_existDefault;
	std::string l_Name = DEFAULT_LAYER_INITIAL_NAME;
	CRenderableObjectsManager* l_auxROM = new CRenderableObjectsManager(l_Name);
	AddResource(l_Name, l_auxROM);
	m_DefaultLayer = l_auxROM;
}

CLayerManager::~CLayerManager()
{
	Destroy();
}

void CLayerManager::Destroy()
{
	//CHECKED_DELETE(m_DefaultLayer);
	CTemplatedVectorMapManager::Destroy();
}

void CLayerManager::Load(const std::string &FileName, const std::string &_LevelId)
{
	m_Filename = FileName;
	
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(FileName.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("renderable_objects");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux != NULL)
			{
				if (l_ElementAux->Name() == std::string("layer"))
				{
					AddLayer(l_ElementAux);
				}
				else if (l_ElementAux->Name() == std::string("instance_mesh"))
				{					
					GetLayer(l_ElementAux)->AddMeshInstance(l_ElementAux, _LevelId);
				}
				else if (l_ElementAux->Name() == std::string("animated_instance_mesh"))
				{
					GetLayer(l_ElementAux)->AddAnimatedInstanceModel(l_ElementAux, _LevelId);
				}
				else if (l_ElementAux->Name() == std::string("particle_instance"))
				{
					GetLayer(l_ElementAux)->AddParticleSystemInstance(l_ElementAux, _LevelId);
				}
				else if (l_ElementAux->Name() == std::string("manchas_instance"))
				{
					GetLayer(l_ElementAux)->AddManchasSystemInstance(l_ElementAux, _LevelId);
				}
				l_ElementAux = l_ElementAux->NextSiblingElement();
			}
		}
	}
}

void CLayerManager::Reload()
{
	//Destroy();
	//Load(m_Filename);

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(m_Filename.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("renderable_objects");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux != NULL)
			{
				if (l_ElementAux->Name() == std::string("layer"))
				{
					AddLayer(l_ElementAux);
				}
				else if (l_ElementAux->Name() == std::string("instance_mesh"))
				{
					GetLayer(l_ElementAux)->AddMeshInstance(l_ElementAux, "");
				}
				else if (l_ElementAux->Name() == std::string("animated_instance_mesh"))
				{
					GetLayer(l_ElementAux)->AddAnimatedInstanceModel(l_ElementAux, "");
				}				
				l_ElementAux = l_ElementAux->NextSiblingElement();
			}
		}
	}
	/*CXMLTreeNode l_XML;

	if (l_XML.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["renderable_objects"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("layer"))
				{
					AddLayer(l_Element, true);
				}
				else if (l_Element.GetName() == std::string("instance_mesh"))
				{
					GetLayer(l_Element)->AddMeshInstance(l_Element, true);
				}
				else if (l_Element.GetName() == std::string("animated_model_mesh"))
				{
					GetLayer(l_Element)->AddAnimatedInstanceModel(l_Element, true);
				}
			}
		}
	}*/
}

void CLayerManager::Update(float ElapsedTime)
{	
	for (size_t i = 0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

void CLayerManager::Render(CRenderManager *RenderManager)
{
	for (size_t i = 0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(RenderManager);
	}
}

void CLayerManager::Render(CRenderManager *RenderManager, const std::string &LayerName)
{
	GetResource(LayerName)->Render(RenderManager); 
}

#ifdef _DEBUG
void CLayerManager::RenderDebug(CRenderManager *RenderManager, const std::string &LayerName)
{
	GetResource(LayerName)->RenderDebug(RenderManager);
}
#endif

CRenderableObjectsManager* CLayerManager::GetLayer(tinyxml2::XMLElement* TreeNode)
{
	const char * l_layerExist = TreeNode->GetPszProperty("layer", "");
	if (l_layerExist != "")
	{
		return GetResource(l_layerExist);
	}
	else
	{
		return m_DefaultLayer;
	}	
}

CRenderableObjectsManager * CLayerManager::GetLayer(std::string _layer)
{
	return GetResource(_layer);
}

CRenderableObjectsManager* CLayerManager::GetLayer()
{
	return m_DefaultLayer;
}

CRenderableObjectsManager* CLayerManager::AddLayer(tinyxml2::XMLElement* TreeNode, bool _Update)
{
	const char * l_existDefault;
	std::string l_Name = TreeNode->GetPszProperty("name");
	l_existDefault = TreeNode->GetPszProperty("default", "");
	CRenderableObjectsManager* l_auxROM;
	if (l_existDefault != "")
	{
		ChageKeyName(m_DefaultLayer->GetName(), l_Name);
		m_DefaultLayer->SetName(l_Name);
		l_auxROM = m_DefaultLayer;
	}
	else
	{
		l_auxROM = new CRenderableObjectsManager(l_Name);
		_Update ? AddUpdateResource(l_Name, l_auxROM) : AddResource(l_Name, l_auxROM);
	}
	return l_auxROM;
}

void CLayerManager::Save()
{
	FILE* l_File;
	if (!fopen_s(&l_File, m_Filename.c_str(), "w"))
	{
		fprintf_s(l_File, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
		fprintf_s(l_File, "<renderable_objects>\n");

		for (size_t i = 0; i < m_ResourcesVector.size(); ++i)
		{
			if (m_DefaultLayer == m_ResourcesVector[i])
				fprintf_s(l_File, "\t<layer name=\"%s\" default=\"true\"/>\n",m_ResourcesVector[i]->GetName().c_str());
			else
				fprintf_s(l_File, "\t<layer name=\"%s\"/>\n", m_ResourcesVector[i]->GetName().c_str());
		}

		for (size_t i = 0; i < m_ResourcesVector.size(); ++i)
		{
			for (size_t j = 0; j < m_ResourcesVector[i]->GetResourcesVector().size(); ++j)
			{
				m_ResourcesVector[i]->GetResourcesVector()[j]->Save(l_File, m_ResourcesVector[i]->GetName());
			}
		}

		fprintf_s(l_File, "</renderable_objects>\n");
		fclose(l_File);
	}
}