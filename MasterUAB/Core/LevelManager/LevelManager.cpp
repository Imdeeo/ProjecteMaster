#include "LevelManager.h"

#include "XML\tinyxml2.h"
#include "Engine\UABEngine.h"

#include "Materials\MaterialManager.h"
#include "StaticMesh\StaticMeshManager.h"
#include "Lights\LightManager.h"
#include "Layers\LayerManager.h"
#include "ScriptManager\ScriptManager.h"
#include "Camera\CameraControllerManager.h"
#include "Cinematics\CinematicManager.h"
#include "Particles\ParticleManager.h"
#include "Bilboards\BilboardManager.h"
#include "Manchas\ManchasManager.h"
#include "IA\AStarManager.h"

#include "StaticMesh\InstanceMesh.h"
#include "StaticMesh\StaticMeshManager.h"
#include "Materials\Material.h"

#include "SceneRender\SceneRendererCommandManager.h"

#include <thread>

CLevelManager::CLevelManager()
{
}

CLevelManager::~CLevelManager()
{
}

void CLevelManager::LoadFile(const std::string &_LevelsFilename)
{
	m_LevelsFileName = _LevelsFilename;
	
	tinyxml2::XMLDocument doc;
	doc.LoadFile(_LevelsFilename.c_str());

	tinyxml2::XMLElement* l_Element;
	l_Element = doc.FirstChildElement("levels")->FirstChildElement("level");
	
	while (l_Element != NULL)
	{
		std::string l_LevelName = l_Element->GetPszProperty("name");
		TLevelInfo l_LevelInfo;
		l_LevelInfo.m_Loaded = false;
		l_LevelInfo.m_ID = l_Element->GetPszProperty("id");
		l_LevelInfo.m_Directory = l_Element->GetPszProperty("directory");
		l_LevelInfo.m_LevelInitLuaFunction = l_Element->GetPszProperty("init_function");
		m_LevelsInfo[l_LevelName] = l_LevelInfo;
		l_Element = l_Element->NextSiblingElement();
	}
}

void CLevelManager::LoadLevelThread(const std::string &_LevelName, bool _Visible, bool _HasToUpdate)
{
	CLevel * l_Level = new CLevel(_LevelName);
	l_Level->Load();
	l_Level->SetVisible(_Visible);
	l_Level->SetHasToUpdate(_HasToUpdate);
	m_LevelsInfo[_LevelName].m_Loaded = true;
	std::vector<CRenderableObjectsManager*> l_LayerVector = l_Level->GetLayerManager()->GetResourcesVector();
	for (size_t i = 0; i < l_LayerVector.size(); i++)
	{
		std::string  l_LayerName = l_LayerVector[i]->GetName();
		if (m_LayersMap.find(l_LayerName) == m_LayersMap.end())
		{
			m_LayersMap[l_LayerName] = std::vector<TLevelLayers*>();
		}
		TLevelLayers* l_LevelLayer = new TLevelLayers();
		l_LevelLayer->m_Layer = l_LayerVector[i];
		l_LevelLayer->m_Visible = l_Level->IsVisible();
		m_LayersMap[l_LayerName].push_back(l_LevelLayer);
	}
}

void CLevelManager::LoadLevel(const std::string &_LevelName, bool _Joinable, bool _Visible, bool _HasToUpdate)
{
	if (m_LevelsInfo[_LevelName].m_Loaded)
	{
		ReloadLevel(_LevelName,_Joinable);
	}
	else
	{
		std::thread t(&CLevelManager::LoadLevelThread,this,_LevelName,_Visible,_HasToUpdate);
		if (_Joinable)
		{
			t.join();
		}
		else
		{
			t.detach();
		}
	}
}

void CLevelManager::ReloadLevel(const std::string &_LevelName,bool joinable)
{
	bool l_Visible = *(m_ResourcesMap[_LevelName].m_Value->IsVisible());
	bool l_HasToUpdate = *(m_ResourcesMap[_LevelName].m_Value->HasToUpdate());
	UnloadLevel(_LevelName);
	LoadLevel(_LevelName,joinable,l_Visible,l_HasToUpdate);
}

void CLevelManager::UnloadLevel(const std::string &_LevelName)
{
	std::map<std::string, std::vector<TLevelLayers*>>::iterator it = m_LayersMap.begin();
	for (; it != m_LayersMap.end(); it++)
	{
		it->second.erase(it->second.begin() + m_ResourcesMap[_LevelName].m_Id);
	}
	RemoveResource(_LevelName);
	m_LevelsInfo[_LevelName].m_Loaded = false;
}

void CLevelManager::Update(float _ElapsedTime)
{
	for (size_t i = 0; i < m_ResourcesVector.size(); i++)
	{
		if (*(m_ResourcesVector[i]->HasToUpdate()))
		{
			m_ResourcesVector[i]->Update(_ElapsedTime);
		}
	}
}

void CLevelManager::ReloadAllLua()
{
	std::map<std::string, TLevelInfo>::iterator l_iterator;
	for (l_iterator = m_LevelsInfo.begin(); l_iterator != m_LevelsInfo.end(); l_iterator++)
	{
		if (l_iterator->second.m_Loaded)
		{
			std::string l_Directory = l_iterator->second.m_Directory;
			std::replace(l_Directory.begin(), l_Directory.end(), '\\', '\/');
			UABEngine.GetScriptManager()->RunCode("levelMainLua(\"" + l_Directory + "\",\"" + l_iterator->first + "\")");
		}
	}
	UABEngine.GetScriptManager()->RunCode("auxReloadLua(\"" + m_ActualLevel + "\")");
}

TLevelInfo CLevelManager::GetLevelInfo(const std::string &_LevelName)
{
	return m_LevelsInfo[_LevelName];
}

std::vector<TLevelLayers*>* CLevelManager::GetCompleteLayer(const std::string &_LayerName)
{
	return &(m_LayersMap[_LayerName]);
}

void CLevelManager::ChooseSceneRenderLevel(const std::string &_LevelId)
{
	m_LevelSceneRenderCommands = _LevelId;
}

void CLevelManager::ExecuteSceneCommands(CRenderManager* _RenderManager)
{
	m_SceneRenderCommandsManager[m_LevelSceneRenderCommands]->Execute(_RenderManager);
}

void CLevelManager::AddSceneCommandsManager(const std::string &_LevelId,CSceneRendererCommandManager* _SceneRendererCommandManager)
{
	m_SceneRenderCommandsManager[_LevelId] = _SceneRendererCommandManager;
}

CSceneRendererCommandManager * CLevelManager::GetSceneRendererCommandManager(const std::string _levelId)
{
	return m_SceneRenderCommandsManager[_levelId];
}

void CLevelManager::ChangeObjectLevel(const std::string &_OldLevelName, const std::string &_NewLevelName, const std::string &_LayerName, const std::string &_ObjectName)
{
	CLevel *l_OldLevel = m_ResourcesMap[_OldLevelName].m_Value;
	CLevel *l_NewLevel = m_ResourcesMap[_NewLevelName].m_Value;

	CRenderableObject* l_Object = l_OldLevel->GetLayer(_LayerName)->GetResource(_ObjectName);
	((CInstanceMesh*)l_Object)->ChangeLevel(_NewLevelName);
	l_NewLevel->GetLayer(_LayerName)->AddResource(_ObjectName, l_Object, _NewLevelName);
	l_OldLevel->GetLayer(_LayerName)->RemoveResource(_ObjectName,false);

	std::string l_CoreName = ((CInstanceMesh*)l_Object)->GetCoreName();
	CStaticMesh* l_Core = l_OldLevel->GetStaticMeshManager()->GetResource(l_CoreName);
	l_Core->m_Level = _NewLevelName;
	l_NewLevel->GetStaticMeshManager()->AddResource(l_CoreName,l_Core,_NewLevelName);
	l_OldLevel->GetStaticMeshManager()->RemoveResource(l_CoreName, false);

	std::vector<CMaterial*> l_Materials = l_Core->GetMaterials();
	std::vector<CMaterial*>l_NewMaterials;
	for (size_t i = 0; i < l_Materials.size(); i++)
	{
		CMaterial *l_Material = l_Materials[i];
		CMaterial *l_MaterialExist = l_NewLevel->GetMaterialManager()->GetResource(l_Material->GetName());
		if (l_MaterialExist == nullptr)
		{
			l_Material = new CMaterial(*(l_Materials[i]));
			l_Material->CopyParameters(l_Materials[i]->GetParameters(),false);
			l_Material->m_Level = _NewLevelName;
			l_NewLevel->GetMaterialManager()->AddResource(l_Material->GetName(), l_Material, _NewLevelName);
		}
		else
		{
			l_Material = l_MaterialExist;
		}
		l_NewMaterials.push_back(l_Material);
		//l_OldLevel->GetMaterialManager()->RemoveResource(l_Material->GetName(),false);
	}
	l_Core->SetMaterials(l_NewMaterials);
}