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
	UABEngine.SetLevelLoaded(m_LevelsInfo[_LevelName].m_ID);
	std::string l_LevelDirectory = m_LevelsInfo[_LevelName].m_Directory;
	UABEngine.GetMaterialManager()->Load(l_LevelDirectory + "\\materials.xml");
	UABEngine.GetParticleManager()->Load(l_LevelDirectory + "\\particles.xml");
	UABEngine.GetBilboardManager()->Load(l_LevelDirectory + "\\particles.xml");
	UABEngine.GetManchasManager()->Load(l_LevelDirectory + "\\cordura.xml");
	UABEngine.GetStaticMeshManager()->Load(l_LevelDirectory + "\\static_meshes.xml");
	UABEngine.GetLayerManager()->Load(l_LevelDirectory + "\\renderable_objects.xml");
	UABEngine.GetLightManager()->Load(l_LevelDirectory + "\\lights.xml");
	UABEngine.GetCinematicManager()->LoadXML(l_LevelDirectory + "\\cinematic.xml");
	UABEngine.GetCameraControllerManager()->Load(l_LevelDirectory + "\\cameras.xml");
	UABEngine.GetAStarManager()->Load(l_LevelDirectory + "\\pathfinding.xml");
	std::string l_LevelDirectoryChangedSlashes = l_LevelDirectory;
	std::replace(l_LevelDirectoryChangedSlashes.begin(), l_LevelDirectoryChangedSlashes.end(), '\\', '\/');
	UABEngine.GetScriptManager()->RunCode("levelMainLua(\"" + l_LevelDirectoryChangedSlashes + "\")");
	m_LevelsInfo[_LevelName].m_Loaded = true;
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
			UABEngine.GetScriptManager()->RunCode("levelMainLua(\"" + l_Directory + "\")");
		}
	}
}
