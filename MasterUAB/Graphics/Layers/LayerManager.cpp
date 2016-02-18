#include "LayerManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "Utils.h"

CLayerManager::CLayerManager():m_DefaultLayer(nullptr){}

CLayerManager::~CLayerManager()
{
	Destroy();
}

void CLayerManager::Destroy()
{
	//CHECKED_DELETE(m_DefaultLayer);
}

void CLayerManager::Load(const std::string &FileName)
{
	Destroy();
	m_Filename = FileName;
	CXMLTreeNode l_XML;
	
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["renderable_objects"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("layer"))
				{
					AddLayer(l_Element);					
				}
				else if (l_Element.GetName() == std::string("instance_mesh"))
				{					
					GetLayer(l_Element)->AddMeshInstance(l_Element);
				}
			}
		}
	}
}

void CLayerManager::Reload()
{
	Destroy();
	Load(m_Filename);
}

void CLayerManager::Update(float ElapsedTime)
{	
	for (int i = 0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

void CLayerManager::Render(CRenderManager *RenderManager)
{
	for (int i = 0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(RenderManager);
	}
}

void CLayerManager::Render(CRenderManager *RenderManager, const std::string &LayerName)
{
	GetResource(LayerName)->Render(RenderManager); 
}

CRenderableObjectsManager* CLayerManager::GetLayer(CXMLTreeNode &Node)
{
	const char * l_layerExist = Node.GetPszProperty("layer");
	if (l_layerExist != NULL)
	{
		return GetResource(l_layerExist);
	}
	else
	{
		return m_DefaultLayer;
	}	
}

CRenderableObjectsManager* CLayerManager::GetLayer()
{
	return m_DefaultLayer;
}

CRenderableObjectsManager* CLayerManager::AddLayer(CXMLTreeNode &TreeNode)
{
	const char * l_existDefault;
	std::string l_Name = TreeNode.GetPszProperty("name");
	CRenderableObjectsManager* l_auxROM = new CRenderableObjectsManager(l_Name);
	AddResource(l_Name, l_auxROM);
	l_existDefault = TreeNode.GetPszProperty("default");
	if (l_existDefault != NULL)
	{
		m_DefaultLayer = l_auxROM;
	}
	return l_auxROM;
}