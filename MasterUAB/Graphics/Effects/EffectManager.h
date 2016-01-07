#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "TemplatedMapManager.h"
#include "Effects\EffectTechnique.h"

#include "Effects\EffectShader.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectParameters.h"
#include <string.h>

class CSceneEffectParameters;
class CAnimatedModelEffectParameters;
class CLightEffectParameters;
class CLight;

class CEffectManager : public CTemplatedMapManager<CEffectTechnique>
{
private:
	CTemplatedMapManager<CEffectVertexShader> m_VertexShaders;
	CTemplatedMapManager<CEffectPixelShader> m_PixelShaders;
	std::string m_Filename;

	void Destroy();
public:
	static CSceneEffectParameters m_SceneParameters;
	static CAnimatedModelEffectParameters m_AnimatedModelEffectParameters;
	static CLightEffectParameters m_LightParameters;
	//static CEffectParameters m_Parameters;

	CEffectManager();
	virtual ~CEffectManager();
		
	void ReloadFile();
	void Reload();
	bool Load(const std::string &Filename);
	CEffectVertexShader * GetVertexShader(const std::string &VertexShader);
	CEffectPixelShader * GetPixelShader(const std::string &PixelShader);

	void SetSceneConstants();
	void SetLightConstants(unsigned int IdLight, CLight *Light);
	void SetLightsConstants(unsigned int MaxLights = MAX_LIGHTS_BY_SHADER);
};

#endif //EFFECT_MANAGER_H