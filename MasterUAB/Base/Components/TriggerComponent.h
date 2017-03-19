#ifndef H_TRIGGER_COMPONENT_H
#define H_TRIGGER_COMPONENT_H

#include "Utils\Named.h"
#include <string>

class CRenderableObject;
class CRenderManager;

class CTriggerComponent 
{
	private:
		std::string m_Name;
		CRenderableObject* m_Owner;
	public:
		CTriggerComponent(const std::string &Name, CRenderableObject *Owner);
		virtual ~CTriggerComponent() {}
		virtual void onCreate(float _ElapsedTime){};
		virtual void onEnter(float _ElapsedTime){};
		virtual void onStay(float _ElapsedTime){};
		virtual void onExit(float _ElapsedTime){};

};

#endif //H_TRIGGER_COMPONENT_H