#ifndef UAB_COMPONENT_MANAGER_H
#define UAB_COMPONENT_MANAGER_H

class CUABComponentManager : public CTemplatedVectorMapManager<CUABComponent>
{
public:
	CUABComponentManager();
	virtual ~CUABComponentManager();
	void Update(float ElapsedTime);
	void Render(CRenderManager &RenderManager);
	void RenderDebug(CRenderManager &RenderManager);
	void AddComponent(const std::string &Name, CComponent *Component);
};

#endif //UAB_COMPONENT_MANAGER_H