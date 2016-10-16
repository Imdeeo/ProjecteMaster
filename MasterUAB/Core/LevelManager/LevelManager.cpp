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

#include "SceneRender\SceneRendererCommandManager.h"

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

void CLevelManager::LoadLevel(const std::string &_LevelName)
{
	if (m_LevelsInfo[_LevelName].m_Loaded)
	{
		ReloadLevel(_LevelName);
	}
	else
	{
		CLevel * l_Level = new CLevel(_LevelName);
		AddResource(_LevelName, l_Level);
		l_Level->Load();
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
}

void CLevelManager::ReloadLevel(const std::string &_LevelName)
{

}
void CLevelManager::UnloadLevel(const std::string &_LevelName)
{

}

void CLevelManager::Update(float _ElapsedTime)
{
	for (size_t i = 0; i < m_ResourcesVector.size(); i++)
	{
		m_ResourcesVector[i]->Update(_ElapsedTime);
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