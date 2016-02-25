#ifndef UAB_COMPONENT_H
#define UAB_COMPONENT_H

class CUABComponent : public CNamed
{
	private:
		CRenderableObject *m_Owner;
	public:
		CUABComponent(const std::string &Name, CRenderableObject *Owner): CNamed(Name), m_Owner(Owner){}
		virtual ~CUABComponent() {}
		virtual void Update(float ElapsedTime) {}
		virtual void Render(CRenderManager &RenderManager) {}
		virtual void RenderDebug(CRenderManager &RenderManager) {}
		UAB_GET_PROPERTY_POINTER(CRenderableObject, Owner);
};

#endif //UAB_COMPONENT_H