#ifndef LEVEL_H
#define LEVEL_H

#include "Utils\Named.h"
#include <string>

class CMaterialManager;
class CParticleManager;
class CManchasManager;
class CStaticMeshManager;
class CLayerManager;
class CLightManager;
class CCinematicManager;
class CCameraControllerManager;
class CAStarManager;
class CGamePlayManager;
class CRenderableObjectsManager;

class CAStar; //DeMoment

class CLevel :	public CNamed
{
private:
	CMaterialManager* m_MaterialManager;
	CParticleManager* m_ParticleManager;
	CManchasManager* m_ManchasManager;
	CStaticMeshManager* m_StaticMeshManager;
	CLayerManager* m_LayerManager;
	CLightManager* m_LightManager;
	CCinematicManager* m_CinematicManager;
	CCameraControllerManager* m_CameraControllerManager;
	//CAStarManager* m_AStarManager;     per quan hi hagi un manager
	CAStar* m_AStarManager;
	CGamePlayManager* m_GamePlayManager;
public:
	CLevel();
	CLevel(const std::string &_Name);
	~CLevel();

	void Load();
	void Reload();
	void Unload();

	CRenderableObjectsManager* GetLayer(const std::string& _LayerName);
};

#endif //LEVEL_H