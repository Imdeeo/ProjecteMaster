#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectShader.h"
#include "Effects\EffectTechnique.h"
#include "Materials\MaterialParameter.h"
#include "SceneEffectParameters.h"
#include "AnimatedModelEffectParameters.h"
#include "LightEffectParameters.h"

#include <string.h>

class CLight;

#define MAX_RAW_DATA_ELEMENTS 64
#define INDEX_SHADOWMAP_TEXTURE 6

class CEffectManager : public CTemplatedMapManager<CEffectTechnique>
{
private:
	CTemplatedMapManager<CEffectVertexShader> m_VertexShaders;
	CTemplatedMapManager<CEffectPixelShader> m_PixelShaders;
	std::string m_Filename;

	static float m_RawData[MAX_RAW_DATA_ELEMENTS];
	
public:
	static CSceneEffectParameters m_SceneParameters;
	static CAnimatedModelEffectParameters m_AnimatedModelEffectParameters;
	static CLightEffectParameters m_LightParameters;

	static void* GetRawData(){ return m_RawData; }
	static void* AddMaterialParameter(CMaterialParameter::TMaterialType _MaterialType);
	//static CEffectParameters m_Parameters;

	static int m_RawDataCount;

	CEffectManager();
	virtual ~CEffectManager();

	void ReloadFile(){};
	void Reload();
	void ReloadShader();
	bool Load(const std::string &Filename);
	CEffectVertexShader * GetVertexShader(const std::string &VertexShader);
	CEffectPixelShader * GetPixelShader(const std::string &PixelShader);

	void SetSceneConstants();
	void SetLightConstants(unsigned int IdLight, CLight *Light);
	void SetLightsConstants(unsigned int MaxLights = MAX_LIGHTS_BY_SHADER);
};

#endif //EFFECT_MANAGER_H