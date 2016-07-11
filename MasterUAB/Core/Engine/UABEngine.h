#ifndef UAB_ENGINE_H
#define UAB_ENGINE_H

#include "Utils.h"

#define LEVEL "2"

class CStaticMeshManager;
class CLayerManager;
class CMaterialManager;
class CEffectManager;
class CTextureManager;
class CLightManager;
class CRenderManager;
class CAnimatedModelsManager;
class CScriptManager;
class CCameraControllerManager;
class CCinematicManager;
class CPhysXManager;
class CRenderableObjectTechniqueManager;
class CSceneRendererCommandManager;
class CParticleManager;
class CGUIManager;
class ISoundManager;
class IVideoManager;
class CGamePlayManager;
class CManchasManager;

#include <string>

class CUABEngine
{
private:
	bool m_LuaReloaded = false;

	static CUABEngine * m_Instance;
	CUABEngine(void);

	CStaticMeshManager * m_StaticMeshManager;
	CLayerManager * m_LayerManager;
	CMaterialManager * m_MaterialManager;
	CEffectManager * m_EffectManager;
	CTextureManager * m_TextureManager;
	CLightManager * m_LightManager;
	CRenderManager * m_RenderManager;
	CAnimatedModelsManager * m_AnimatedModelsManager;
	CScriptManager * m_ScriptManager;
	CCameraControllerManager * m_CameraControllerManager;
	CCinematicManager * m_CinematicManager;
	CPhysXManager * m_PhysXManager;
	CRenderableObjectTechniqueManager* m_RenderableObjectTechniqueManager;
	CSceneRendererCommandManager* m_SceneRendererCommandManager;
	CParticleManager* m_ParticleManager;
	CGUIManager* m_GUIManager;
	ISoundManager* m_SoundManager;
	IVideoManager* m_VideoManager;
	CGamePlayManager* m_GamePlayManager;
	CManchasManager* m_ManchasManager;
	std::string m_LevelLoaded;
	int m_CurrentCamera_vision;
	
public:
	CStaticMeshManager * GetStaticMeshManager()const;
	CLayerManager * GetLayerManager()const;
	CMaterialManager * GetMaterialManager()const;
	CEffectManager * GetEffectManager()const;
	CTextureManager * GetTextureManager()const;
	CLightManager * GetLightManager()const;
	CRenderManager * GetRenderManager()const;
	CAnimatedModelsManager * GetAnimatedModelsManager()const;
	CScriptManager * GetScriptManager()const;
	CCameraControllerManager * GetCameraControllerManager()const;
	CCinematicManager * GetCinematicManager()const;
	CPhysXManager * GetPhysXManager()const;
	CRenderableObjectTechniqueManager * GetRenderableObjectTechniqueManager()const;
	CSceneRendererCommandManager * GetSceneRendererCommandManager()const;
	CParticleManager * GetParticleManager()const;
	CGUIManager * GetGUIManager() const;
	ISoundManager * GetSoundManager()const;
	IVideoManager * GetVideoManager() const;
	CGamePlayManager * GetGamePlayManager()const;
	CManchasManager * GetManchasManager()const;
	std::string GetLevelLoaded()const{return m_LevelLoaded;}
	void LoadLevelXML(std::string filename);
	static CUABEngine * GetInstance();
	virtual ~CUABEngine(void);
	void Destroy();
	void Init();
	void Update(float _ElapsedTime);
	void SwitchCamera();
	void ChangeCameraVision();
	void Quit();
	UAB_BUILD_GET_SET(bool, FrustumActive);
	UAB_BUILD_GET_SET(float, TimeScale);
	UAB_BUILD_GET_SET(bool, Pause);

	bool GetLuaReloaded(){ return m_LuaReloaded; }
	void LuaIsReloaded(){ m_LuaReloaded = true; }
};

#define UABEngine (*(CUABEngine::GetInstance()))

#endif //UAB_ENGINE_H
