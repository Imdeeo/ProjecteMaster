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
	std::string m_FileName;
	Vect4f m_AmbientLight;
	bool m_RenderLights;
public:
	CLightManager();
	virtual ~CLightManager();
	bool Load(const std::string &FileName);
	bool Render(CRenderManager *RenderManager);
	bool Reload();
	UAB_GET_PROPERTY(Vect4f,AmbientLight);
	void GetRenderLights(bool _RenderLights){ m_RenderLights = _RenderLights; }
	bool IsRenderLights(){ return m_RenderLights; }
	bool SwitchRenderLights(){ m_RenderLights = !m_RenderLights; return m_RenderLights; }
};

#endif //LIGHT_MANAGER_H

