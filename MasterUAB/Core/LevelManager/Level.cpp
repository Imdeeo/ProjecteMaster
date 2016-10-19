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
#include "SceneRender\SceneRendererCommandManager.h"
#include "Bilboards\BilboardManager.h"

#include "IA\AStarManager.h"

#include "LevelManager\LevelManager.h"


CLevel::CLevel() :CNamed("")
{
	m_MaterialManager = new CMaterialManager();
	m_ParticleManager = new CParticleManager();
	m_StaticMeshManager = new CStaticMeshManager();
	m_LightManager = new CLightManager();
	m_LayerManager = new CLayerManager();
	m_CinematicManager = new CCinematicManager();
	m_GamePlayManager = new CGamePlayManager();
	m_ManchasManager = new CManchasManager();
	m_AStarManager = new CAStarManager();
}

CLevel::CLevel(const std::string &_Name) :CNamed(_Name)
{
	m_MaterialManager = new CMaterialManager();
	m_ParticleManager = new CParticleManager();
	m_StaticMeshManager = new CStaticMeshManager();
	m_LightManager = new CLightManager();
	m_BilboardManager = new CBilboardManager();
	m_LayerManager = new CLayerManager();
	m_CinematicManager = new CCinematicManager();
	m_GamePlayManager = new CGamePlayManager();
	m_ManchasManager = new CManchasManager();
	m_AStarManager = new CAStarManager();
}


CLevel::~CLevel()
{
	UtilsLog("\n\n\n\n");
	UtilsLog("Destroy Level");
	UABEngine.GetScriptManager()->RunCode("DestroyLevelGamePlayManager(\""+m_Name+"\")");
	CHECKED_DELETE(m_GamePlayManager);
	CHECKED_DELETE(m_AStarManager);
	CHECKED_DELETE(m_CinematicManager);
	CHECKED_DELETE(m_LightManager);
	CHECKED_DELETE(m_LayerManager);
	CHECKED_DELETE(m_StaticMeshManager);
	CHECKED_DELETE(m_BilboardManager);
	CHECKED_DELETE(m_ManchasManager);
	CHECKED_DELETE(m_ParticleManager);
	CHECKED_DELETE(m_MaterialManager);
	UtilsLog("End Destroy Level");
	UtilsLog("\n\n\n\n");
}

void CLevel::Load()
{
	std::string l_LevelDirectory = UABEngine.GetLevelManager()->GetLevelInfo(m_Name).m_Directory;
	m_MaterialManager->Load(l_LevelDirectory + "\\materials.xml", "", m_Name);
	m_ParticleManager->Load(l_LevelDirectory + "\\particles.xml", m_Name);
	m_ManchasManager->Load(l_LevelDirectory + "\\cordura.xml", m_Name);
	m_BilboardManager->Load(l_LevelDirectory + "\\particles.xml", m_Name);
	m_StaticMeshManager->Load(l_LevelDirectory + "\\static_meshes.xml", m_Name);
	m_LayerManager->Load(l_LevelDirectory + "\\renderable_objects.xml", m_Name);
	m_LightManager->Load(l_LevelDirectory + "\\lights.xml", m_Name);
	m_CinematicManager->LoadXML(l_LevelDirectory + "\\cinematic.xml", m_Name);
	UABEngine.GetCameraControllerManager()->Load(l_LevelDirectory + "\\cameras.xml", m_Name);
	m_AStarManager->Load(l_LevelDirectory + "\\pathfinding.xml",m_Name);
	std::string l_LevelDirectoryChangedSlashes = l_LevelDirectory;
	std::replace(l_LevelDirectoryChangedSlashes.begin(), l_LevelDirectoryChangedSlashes.end(), '\\', '\/');
	CSceneRendererCommandManager* l_SceneRendererCommandManager = new CSceneRendererCommandManager();
	l_SceneRendererCommandManager->Load(l_LevelDirectory + "\\scene_renderer_commands.xml", m_Name);
	UABEngine.GetLevelManager()->AddSceneCommandsManager(m_Name,l_SceneRendererCommandManager);
	UABEngine.GetScriptManager()->RunCode("levelMainLua(\"" + l_LevelDirectoryChangedSlashes + "\",\"" + m_Name + "\")");
}

void CLevel::Reload()
{
}

void CLevel::Unload()
{
}

void CLevel::Update(float _ElapsedTime)
{
	m_LayerManager->Update(_ElapsedTime);
	m_CinematicManager->Update(_ElapsedTime);
	m_GamePlayManager->Update(_ElapsedTime);
}

CRenderableObjectsManager* CLevel::GetLayer(const std::string &_LayerName)
{
	return m_LayerManager->GetLayer(_LayerName);
}

CStaticMeshManager * CLevel::GetStaticMeshManager() const
{
	return m_StaticMeshManager;
}
CLayerManager * CLevel::GetLayerManager() const
{
	return m_LayerManager;
}
CMaterialManager * CLevel::GetMaterialManager() const
{
	return m_MaterialManager;
}
CLightManager * CLevel::GetLightManager() const
{
	return m_LightManager;
}
CCinematicManager * CLevel::GetCinematicManager() const
{
	return m_CinematicManager;
}
CParticleManager * CLevel::GetParticleManager() const
{
	return m_ParticleManager;
}
CGamePlayManager * CLevel::GetGamePlayManager()const
{
	return m_GamePlayManager;
}
CManchasManager * CLevel::GetManchasManager() const
{
	return m_ManchasManager;
}
CAStarManager * CLevel::GetAStarManager() const
{
	return m_AStarManager;
}
CBilboardManager * CLevel::GetBilboardManager() const
{
	return m_BilboardManager;
}

bool* CLevel::IsVisible()
{
	return &m_Visible;
}

bool* CLevel::HasToUpdate()
{
	return &m_HasToUpdate;
}

void CLevel::SetVisible(bool _Visible)
{
	m_Visible = _Visible;
}

void CLevel::SetHasToUpdate(bool _HasToUpdate)
{
	m_HasToUpdate = _HasToUpdate;
}