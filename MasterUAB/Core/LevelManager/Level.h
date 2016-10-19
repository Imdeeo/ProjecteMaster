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
class CBilboardManager;

class CAStarManager;

class CLevel :	public CNamed
{
private:
	bool m_HasToUpdate;
	bool m_Visible;

	CMaterialManager* m_MaterialManager;
	CParticleManager* m_ParticleManager;
	CManchasManager* m_ManchasManager;
	CStaticMeshManager* m_StaticMeshManager;
	CLayerManager* m_LayerManager;
	CLightManager* m_LightManager;
	CCinematicManager* m_CinematicManager;
	CAStarManager* m_AStarManager;
	CGamePlayManager* m_GamePlayManager;
	CBilboardManager* m_BilboardManager;
public:
	CLevel();
	CLevel(const std::string &_Name);
	~CLevel();

	void Load();
	void Reload();
	void Unload();
	
	void Update(float _ElapsedTime);

	bool* IsVisible();
	bool* HasToUpdate();
	void SetVisible(bool _Visible);
	void SetHasToUpdate(bool _HasToUpdate);

	CStaticMeshManager * GetStaticMeshManager() const;
	CLayerManager * GetLayerManager() const;
	CMaterialManager * GetMaterialManager() const;
	CLightManager * GetLightManager() const;
	CCinematicManager * GetCinematicManager() const;
	CParticleManager * GetParticleManager() const;
	CGamePlayManager * GetGamePlayManager()const;
	CManchasManager * GetManchasManager() const;
	CAStarManager * GetAStarManager() const;
	CBilboardManager * GetBilboardManager() const;

	CRenderableObjectsManager* GetLayer(const std::string& _LayerName);
};

#endif //LEVEL_H