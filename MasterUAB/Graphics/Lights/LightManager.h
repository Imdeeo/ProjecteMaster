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
	Vect4f		m_FogColor;
	bool  		m_FogEnabled;

public:
	CLightManager();
	virtual ~CLightManager();
	bool Load(const std::string &FileName);
	bool CreateNewLight(std::string _name, std::string _type);
#ifdef _DEBUG
	bool Render(CRenderManager *RenderManager);
#endif

	bool Reload();
	Vect4f GetAmbientLight(){ return m_AmbientLight; }
	float  GetFogMaxAttenuation(){return m_FogMaxAttenuation; }
	float  GetFogStart(){ return m_FogStart; }
	float  GetFogEnd(){ return m_FogEnd; }
	Vect4f GetFogColor(){ return m_FogColor; }
	bool   GetFogEnabled(){ return m_FogEnabled; }
	void GetRenderLights(bool _RenderLights){ m_RenderLights = _RenderLights; }
	bool IsRenderLights(){ return m_RenderLights; }
	bool SwitchRenderLights(){ m_RenderLights = !m_RenderLights; return m_RenderLights; }
	void Save();
	CEmptyPointerClass* GetFogColorLuaAdress()
	{
		return (CEmptyPointerClass*)&m_FogColor;
	}
	CEmptyPointerClass* GetFogStartRangeAttenuattionLuaAdress()
	{
		return (CEmptyPointerClass*)&m_FogStart;
	}

	CEmptyPointerClass* GetFogEndRangeAttenuattionLuaAdress()
	{
		return (CEmptyPointerClass*)&m_FogEnd;
	}
	CEmptyPointerClass* GetFogMaxAttenuattionLuaAdress()
	{
		return (CEmptyPointerClass*)&m_FogMaxAttenuation;
	}

};

#endif //LIGHT_MANAGER_H

