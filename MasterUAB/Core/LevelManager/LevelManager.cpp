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
#include "Manchas\ManchasManager.h"
#include "IA\AStar.h"


CLevelManager::CLevelManager()
{
}

CLevelManager::~CLevelManager()
{
}

void CLevelManager::LoadFile(const std::string &_LevelsFilename)
{
	m_LayerManagerArray = UABEngine.GetLayerManager();

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
		UABEngine.SetLevelLoaded(m_LevelsInfo[_LevelName].m_ID);
		CLevel * l_Level = new CLevel(_LevelName);
		l_Level->Load();
		AddResource(_LevelName, l_Level);
		m_LevelsInfo[_LevelName].m_Loaded = true;
	}
}

void CLevelManager::ReloadLevel(const std::string &_LevelName)
{

}
void CLevelManager::UnloadLevel(const std::string &_LevelName)
{

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
			UABEngine.GetScriptManager()->RunCode("levelMainLua(\"" + l_Directory + "\",\"" + l_iterator->second.m_ID + "\")");
		}
	}
}

TLevelInfo CLevelManager::GetLevelInfo(std::string &_LevelName)
{
	return m_LevelsInfo[_LevelName];
}

std::vector<CRenderableObjectsManager*> CLevelManager::GetCompleteLayer(const std::string &_LayerName)
{
	std::vector<CRenderableObjectsManager*> l_RenderableObjectVector;
	for (size_t i = 0; i < m_ResourcesVector.size(); i++)
	{
		l_RenderableObjectVector.push_back(m_ResourcesVector[i]->GetLayer(_LayerName));
	}
	return l_RenderableObjectVector;
}