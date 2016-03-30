#ifndef UAB_ENGINE_H
#define UAB_ENGINE_H

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
class CCinematic;
class CPhysXManager;
class CRenderableObjectTechniqueManager;
class CSceneRendererCommandManager;
class CParticleManager;
class CGUIManager;

#include <string>

class CUABEngine
{
private:

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
	CCinematic * m_Cinematic;
	CPhysXManager * m_PhysXManager;
	CRenderableObjectTechniqueManager* m_RenderableObjectTechniqueManager;
	CSceneRendererCommandManager* m_SceneRendererCommandManager;
	CParticleManager* m_ParticleManager;
	CGUIManager* m_GUIManager;

	std::string m_LevelLoaded;
	
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
	CCinematic * GetCinematic()const;
	CPhysXManager * GetPhysXManager()const;
	CRenderableObjectTechniqueManager * GetRenderableObjectTechniqueManager()const;
	CSceneRendererCommandManager * GetSceneRendererCommandManager()const;
	CParticleManager * GetParticleManager()const;
	CGUIManager * GetGUIManager() const;

	std::string GetLevelLoaded()const{return m_LevelLoaded;}
	
	void LoadLevelXML(std::string filename);
public:

	static CUABEngine * GetInstance();
	virtual ~CUABEngine(void);

	void Destroy();
	void Init();

};

#define UABEngine (*(CUABEngine::GetInstance()))

#endif //UAB_ENGINE_H
