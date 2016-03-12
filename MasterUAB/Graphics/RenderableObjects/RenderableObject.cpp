#include "RenderableObject.h"

#include "Components\UABComponentManager.h"
#include "Utils.h"

CRenderableObject::CRenderableObject() :CNamed("") {
	m_ComponentManager = new CUABComponentManager();
	m_DebugRender = true;
};

CRenderableObject::CRenderableObject(const CXMLTreeNode &TreeNode) :C3DElement(TreeNode), CNamed(TreeNode){
	m_ComponentManager = new CUABComponentManager();
	m_DebugRender = true;
};

CRenderableObject::~CRenderableObject()
{
	CHECKED_DELETE(m_ComponentManager);
}

void CRenderableObject::Update(float ElapsedTime)
{
	m_ComponentManager->Update(ElapsedTime);
}
void CRenderableObject::Render(CRenderManager *RM)
{
	m_ComponentManager->Render(*RM);
}
void CRenderableObject::RenderDebug(CRenderManager *RM)
{
	m_ComponentManager->RenderDebug(*RM);
}

void CRenderableObject::SetComponentManager(CUABComponentManager* _ComponentManager)
{
	m_ComponentManager = _ComponentManager;
}
CUABComponentManager* CRenderableObject::GetComponentManager()const
{
	return m_ComponentManager;
}

CRenderableObject & CRenderableObject::operator=(CRenderableObject & _RenderableObject)
{
	*((C3DElement*)this) = (C3DElement)_RenderableObject;
	*(m_ComponentManager) = *(_RenderableObject.m_ComponentManager);
	m_DebugRender = _RenderableObject.m_DebugRender;
	m_Name = _RenderableObject.m_Name;
	m_DebugRender = _RenderableObject.m_DebugRender;
	return *this;
}
