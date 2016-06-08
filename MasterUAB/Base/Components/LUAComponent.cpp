#include "LUAComponent.h"
#include "RenderableObjects\RenderableObject.h"

CLUAComponent::CLUAComponent(const std::string &Name, CRenderableObject *Owner, const	std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate,
	const std::string &FnOnRender, const std::string &FnOnRenderDebug) : CScriptedComponent(Name, Owner, FnOnCreate, FnOnDestroy, FnOnUpdate, FnOnRender, FnOnRenderDebug)
{
	
}

CLUAComponent * CLUAComponent::CreateLUAComponent(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy,
	const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug)
{
	return new CLUAComponent(Name, Owner, FnOnCreate, FnOnDestroy, FnOnUpdate, FnOnRender, FnOnRenderDebug);
}
