#ifndef H_LUA_COMPONENT_H
#define H_LUA_COMPONENT_H

#include "Utils\Named.h"
#include <string>

class CRenderableObject;
class CRenderManager;

class CLUAComponent 
{
	private:
		std::string m_Name;
		CRenderableObject* m_Owner;
	public:
		CLUAComponent(const std::string &Name, CRenderableObject *Owner);
		virtual ~CLUAComponent() {}
		virtual void Update(float _ElapsedTime){};

};

#endif //H_LUA_COMPONENT_H