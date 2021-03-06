#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"

#include "Math\Vector4.h"
#include <string>
#include "Lights\Light.h"

class CRenderManager;
class CLevel;

class CLightManager : public CTemplatedLevelVectorMapManager<CLight>
{
private:
	std::string	m_FileName;
	std::string m_LevelName;
	Vect4f		m_AmbientLight;
	bool		m_RenderLights;

public:
	CLightManager();
	virtual ~CLightManager();
	bool Load(const std::string &FileName, CLevel* _Level);
	bool CreateNewLight(std::string _name, std::string _type);

	bool RenderAux(CRenderManager *RenderManager);
	bool Render(CRenderManager *RenderManager);

	bool Reload();
	Vect4f GetAmbientLight(){ return m_AmbientLight; }	
	void SetRenderLights(bool _RenderLights){ m_RenderLights = _RenderLights; }
	bool IsRenderLights(){ return m_RenderLights; }
	bool SwitchRenderLights(){ m_RenderLights = !m_RenderLights; return m_RenderLights; }
	void Save();
};

#endif //LIGHT_MANAGER_H

