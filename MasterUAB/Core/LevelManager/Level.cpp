#include "Level.h"

#include "Engine\UABEngine.h"
#include "ScriptManager\ScriptManager.h"

#include "Materials\MaterialManager.h"
#include "Camera\CameraControllerManager.h"
#include "Cinematics\CinematicManager.h"
#include "Particles\ParticleManager.h"
#include "Manchas\ManchasManager.h"
#include "StaticMesh\StaticMeshManager.h"
#include "Layers\LayerManager.h"
#include "Lights\LightManager.h"
#include "GamePlayManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"

#include "IA\AStar.h"

#include "LevelManager\LevelManager.h"


CLevel::CLevel() :CNamed("")
{
	m_MaterialManager = new CMaterialManager();
	m_ParticleManager = new CParticleManager();
	m_StaticMeshManager = new CStaticMeshManager();
	m_LightManager = new CLightManager();
	m_LayerManager = new CLayerManager();
	m_CameraControllerManager = new CCameraControllerManager();
	m_CinematicManager = new CCinematicManager();
	m_GamePlayManager = new CGamePlayManager();
	m_ManchasManager = new CManchasManager();
	m_AStarManager = new CAStar();
}

CLevel::CLevel(const std::string &_Name) :CNamed(_Name)
{
	m_MaterialManager = new CMaterialManager();
	m_ParticleManager = new CParticleManager();
	m_StaticMeshManager = new CStaticMeshManager();
	m_LightManager = new CLightManager();
	m_LayerManager = new CLayerManager();
	m_CameraControllerManager = new CCameraControllerManager();
	m_CinematicManager = new CCinematicManager();
	m_GamePlayManager = new CGamePlayManager();
	m_ManchasManager = new CManchasManager();
	m_AStarManager = new CAStar();
}


CLevel::~CLevel()
{
}

void CLevel::Load()
{
	std::string l_LevelDirectory = UABEngine.GetLevelManager()->GetLevelInfo(m_Name).m_Directory;
	m_MaterialManager->Load(l_LevelDirectory + "\\materials.xml", "", m_Name);
	m_ParticleManager->Load(l_LevelDirectory + "\\particles.xml", m_Name);
	m_ManchasManager->Load(l_LevelDirectory + "\\cordura.xml", m_Name);
	m_StaticMeshManager->Load(l_LevelDirectory + "\\static_meshes.xml", m_Name);
	m_LayerManager->Load(l_LevelDirectory + "\\renderable_objects.xml", m_Name);
	m_LightManager->Load(l_LevelDirectory + "\\lights.xml", m_Name);
	m_CinematicManager->LoadXML(l_LevelDirectory + "\\cinematic.xml", m_Name);
	m_CameraControllerManager->Load(l_LevelDirectory + "\\cameras.xml", m_Name);
	m_AStarManager->LoadMap(l_LevelDirectory + "\\pathfinding.xml");
	std::string l_LevelDirectoryChangedSlashes = l_LevelDirectory;
	std::replace(l_LevelDirectoryChangedSlashes.begin(), l_LevelDirectoryChangedSlashes.end(), '\\', '\/');
	UABEngine.GetScriptManager()->RunCode("levelMainLua(\"" + l_LevelDirectoryChangedSlashes + "\",\"" + m_Name + "\")");
}

void Reload()
{
}

void Unload()
{
}

CRenderableObjectsManager* CLevel::GetLayer(const std::string &_LayerName)
{
	return m_LayerManager->GetLayer(_LayerName);
}