#include "LUAComponent.h"
#include "RenderableObjects\RenderableObject.h"

CLUAComponent::CLUAComponent(const std::string &Name, CRenderableObject *Owner)
{
	m_Name = Name;
	m_Owner = Owner;
}

