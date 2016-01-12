#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "TemplatedVectorMapManager.h"

#include "Math\Vector4.h"
#include <string>
#include "Lights\Light.h"

class CRenderManager;

class CLightManager : public CTemplatedVectorMapManager<CLight>
{
private:
	std::string m_FileName;
	Vect4f m_AmbientLight;
public:
	CLightManager();
	virtual ~CLightManager();
	bool Load(const std::string &FileName);
	bool Render(CRenderManager *RenderManager);
	bool Reload();
	UAB_GET_PROPERTY(Vect4f,AmbientLight);
};

#endif //LIGHT_MANAGER_H

