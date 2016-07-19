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
	void GetRenderLights(bool _RenderLights){ m_RenderLights = _RenderLights; }
	bool IsRenderLights(){ return m_RenderLights; }
	bool SwitchRenderLights(){ m_RenderLights = !m_RenderLights; return m_RenderLights; }
	void Save();
};

#endif //LIGHT_MANAGER_H

