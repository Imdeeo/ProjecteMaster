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
	
	void Update(float _ElapsedTime);

	CStaticMeshManager * GetStaticMeshManager() const;
	CLayerManager * GetLayerManager() const;
	CMaterialManager * GetMaterialManager() const;
	CLightManager * GetLightManager() const;
	CCinematicManager * GetCinematicManager() const;
	CParticleManager * GetParticleManager() const;
	CGamePlayManager * GetGamePlayManager()const;
	CManchasManager * GetManchasManager() const;
	CAStar * GetAStarManager() const;

	CRenderableObjectsManager* GetLayer(const std::string& _LayerName);
};

#endif //LEVEL_H