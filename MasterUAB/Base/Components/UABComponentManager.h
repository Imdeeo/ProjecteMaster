#ifndef UAB_COMPONENT_MANAGER_H
#define UAB_COMPONENT_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include <string>

class CRenderManager;
class CUABComponent;

class CUABComponentManager
{
protected:
	std::vector<CUABComponent *> m_Resources;
public:	
	CUABComponentManager();
	virtual ~CUABComponentManager();
	void Update(float ElapsedTime);
	void Destroy();
};

#endif //UAB_COMPONENT_MANAGER_H