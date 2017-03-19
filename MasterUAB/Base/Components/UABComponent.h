#ifndef UAB_COMPONENT_H
#define UAB_COMPONENT_H

#include "Utils\Named.h"
#include <string>

class CRenderableObject;
class CRenderManager;

class CUABComponent : public CNamed
{
	private:
		CRenderableObject *m_Owner;
	public:
		CUABComponent(const std::string &Name, CRenderableObject *Owner);
		virtual ~CUABComponent() {}
		virtual void Update(float ElapsedTime) {}
		virtual void Render(CRenderManager &RenderManager) {}
		virtual void RenderDebug(CRenderManager &RenderManager) {}
		CRenderableObject * GetOwner()const;
};

#endif //UAB_COMPONENT_H