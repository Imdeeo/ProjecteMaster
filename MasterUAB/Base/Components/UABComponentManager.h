#ifndef UAB_COMPONENT_MANAGER_H
#define UAB_COMPONENT_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "UABComponent.h"
#include <string>

class CRenderManager;

class CUABComponentManager
{
protected:
	std::vector<CUABComponent *> m_Resources;
public:	
	CUABComponentManager();
	virtual ~CUABComponentManager();
	void Update(float ElapsedTime);
	
	//void Render(CRenderManager &RenderManager);
	//void RenderDebug(CRenderManager &RenderManager);
	//void AddComponent(const std::string &Name, CUABComponent *Component);
};

#endif //UAB_COMPONENT_MANAGER_H