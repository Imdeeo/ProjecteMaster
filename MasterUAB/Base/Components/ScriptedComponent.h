#ifndef UAB_SCRIPTED_COMPONENT_H
#define UAB_SCRIPTED_COMPONENT_H

#include "UABComponent.h"
#include <string>

class CRenderManager;

class CScriptedComponent : public CUABComponent
{
private:
	std::string m_FnOnCreate;
	std::string m_FnOnDestroy;
	std::string m_FnOnUpdate;
	std::string m_FnOnRender;
	std::string m_FnOnRenderDebug;
public:
	CScriptedComponent(const std::string &Name, CRenderableObject *Owner, const	std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate, 
	const std::string &FnOnRender, const std::string &FnOnRenderDebug);
	virtual ~CScriptedComponent();
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager &RenderManager);
	virtual void RenderDebug(CRenderManager &RenderManager);
	static CScriptedComponent * CreateScriptedComponent(const std::string &Name,CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy, 
	const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug);
};

#endif //UAB_SCRIPTED_COMPONENT_H