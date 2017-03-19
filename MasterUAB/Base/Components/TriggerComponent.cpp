#include "TriggerComponent.h"
#include "RenderableObjects\RenderableObject.h"

CTriggerComponent::CTriggerComponent(const std::string &Name, CRenderableObject *Owner)
{
	m_Name = Name;
	m_Owner = Owner;
}

