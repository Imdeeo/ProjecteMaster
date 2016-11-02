#ifndef UAB_ENGINE_H
#define UAB_ENGINE_H

#include "Utils.h"
#include <random>

#define LEVEL "2"

class CInputManager;
class CMaterialManager;
class CEffectManager;
class CTextureManager;
class CRenderManager;
class CAnimatedModelsManager;
class CScriptManager;
class CCameraControllerManager;
class CPhysXManager;
class CRenderableObjectTechniqueManager;
class CGUIManager;
class ISoundManager;
class IVideoManager;
class CLevelManager;
class CColor;
class CRenderableObject;
class CEffectTechnique;
class CLoadScreenManager;
class CMutexManager;

#include <string>

class CUABEngine
{
private:
	std::mt19937 m_RandomEngine;
	std::uniform_real_distribution<float> m_UnitDistribution;
	std::random_device rnd;
	bool m_LuaReloaded = false;

	static CUABEngine * m_Instance;
	CUABEngine(void);

	CInputManager * m_InputManager;
	CMaterialManager * m_MaterialManager;
	CEffectManager * m_EffectManager;
	CTextureManager * m_TextureManager;
	CRenderManager * m_RenderManager;
	CAnimatedModelsManager * m_AnimatedModelsManager;
	CScriptManager * m_ScriptManager;
	CCameraControllerManager * m_CameraControllerManager;
	CPhysXManager * m_PhysXManager;
	CRenderableObjectTechniqueManager* m_RenderableObjectTechniqueManager;

	CMutexManager* m_MutexManager;

	CGUIManager* m_GUIManager;
	ISoundManager* m_SoundManager;
	IVideoManager* m_VideoManager;
	CLevelManager* m_LevelManager;
	CLoadScreenManager* m_LoadScreenManager;
	std::string m_LevelLoaded;
	int m_CurrentCamera_vision;
	bool m_ActiveConsole;

public:
	CMutexManager * GetMutexManager() const;
	CInputManager * GetInputManager() const;
	CMaterialManager * GetMaterialManager() const;
	CEffectManager * GetEffectManager() const;
	CTextureManager * GetTextureManager() const;
	CRenderManager * GetRenderManager() const;
	CAnimatedModelsManager * GetAnimatedModelsManager() const;
	CScriptManager * GetScriptManager() const;
	CCameraControllerManager * GetCameraControllerManager() const;
	CPhysXManager * GetPhysXManager() const;
	CRenderableObjectTechniqueManager * GetRenderableObjectTechniqueManager() const;
	CGUIManager * GetGUIManager() const;
	ISoundManager * GetSoundManager() const;
	IVideoManager * GetVideoManager() const;
	CLevelManager * GetLevelManager()const;
	static CUABEngine * GetInstance();
	virtual ~CUABEngine(void);
	void Destroy();
	void Init();
	void Update(float _ElapsedTime);
	void SwitchCamera();
	void ChangeCameraVision();
	void Quit();
	void ReloadLUA();
	void Consola(float _x, float _y, float _w, float _h);
	UAB_BUILD_GET_SET(bool, FrustumActive);
	UAB_BUILD_GET_SET(float, TimeScale);
	UAB_BUILD_GET_SET(bool, Pause);
	UAB_BUILD_GET_SET(bool, VideoPause);
	bool GetActiveConsole() { return m_ActiveConsole; };
	void SetActiveConsole(bool _ActiveConsole);
	bool GetLuaReloaded(){ return m_LuaReloaded; }
	void LuaIsReloaded(){ m_LuaReloaded = true; }	
	float GetRandomValue(float min, float max);
	Vect3f GetRandomValue(Vect3f min, Vect3f max);
	float GetRandomValue(Vect2f value);
	CColor GetRandomValue(CColor min, CColor max);
	int GetTypeParticle(CRenderableObject* _RO);
};

#define UABEngine (*(CUABEngine::GetInstance()))

#endif //UAB_ENGINE_H
