#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"

#include "Math\Vector4.h"
#include <string>
#include "Lights\Light.h"

class CRenderManager;

class CLightManager : public CTemplatedVectorMapManager<CLight>
{
private:
	std::string	m_FileName;
	Vect4f		m_AmbientLight;
	bool		m_RenderLights;

	float 		m_FogMaxAttenuation;
	float 		m_FogStart;
	float 		m_FogEnd;
	Vect3f		m_FogColor;
	bool  		m_FogEnabled;

public:
	CLightManager();
	virtual ~CLightManager();
	bool Load(const std::string &FileName);

#ifdef _DEBUG
	bool Render(CRenderManager *RenderManager);
#endif

	bool Reload();
	Vect4f GetAmbientLight(){ return m_AmbientLight; }
	float  GetFogMaxAttenuation(){return m_FogMaxAttenuation; }
	float  GetFogStart(){ return m_FogStart; }
	float  GetFogEnd(){ return m_FogEnd; }
	Vect3f GetFogColor(){ return m_FogColor; }
	bool   GetFogEnabled(){ return m_FogEnabled; }
	void GetRenderLights(bool _RenderLights){ m_RenderLights = _RenderLights; }
	bool IsRenderLights(){ return m_RenderLights; }
	bool SwitchRenderLights(){ m_RenderLights = !m_RenderLights; return m_RenderLights; }
	void Save();
};

#endif //LIGHT_MANAGER_H

