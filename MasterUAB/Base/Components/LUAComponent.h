#ifndef H_LUA_COMPONENT_H
#define H_LUA_COMPONENT_H

#include "Utils\Named.h"
#include <string>
#include "ScriptedComponent.h"

class CRenderableObject;
class CRenderManager;

class CLUAComponent : public CScriptedComponent
{
	private:
	public:
		CLUAComponent(const std::string &Name, CRenderableObject *Owner, const	std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate,
			const std::string &FnOnRender, const std::string &FnOnRenderDebug);
		virtual ~CLUAComponent() {}
		static CLUAComponent * CreateLUAComponent(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy,
		const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug);
};

#endif //H_LUA_COMPONENT_H