#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "TemplatedVectorMapManager.h"

#include <string>
#include "Lights\Light.h"

class CRenderManager;

class CLightManager : public CTemplatedVectorMapManager<CLight>
{
private:
	std::string m_FileName;
public:
	CLightManager();
	virtual ~CLightManager();
	bool Load(const std::string &FileName);
	bool Render(CRenderManager *RenderManager);
};

#endif //LIGHT_MANAGER_H

