#include "UABComponent.h"
#include "RenderableObjects\RenderableObject.h"

CUABComponent::CUABComponent(const std::string &Name, CRenderableObject *Owner) : CNamed(Name), m_Owner(Owner){}

CRenderableObject * CUABComponent::GetOwner()const
{
	return m_Owner;
}