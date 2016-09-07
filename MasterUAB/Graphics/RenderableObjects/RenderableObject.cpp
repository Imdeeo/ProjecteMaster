#include "RenderableObject.h"

#include "Components\UABComponentManager.h"
#include "Utils.h"

CRenderableObject::CRenderableObject() :CNamed("") {
	m_DebugRender = true;
};

CRenderableObject::CRenderableObject(tinyxml2::XMLElement* TreeNode) :C3DElement(TreeNode), CNamed(TreeNode){
	m_DebugRender = true;
};

CRenderableObject::~CRenderableObject()
{
}


CRenderableObject & CRenderableObject::operator=(CRenderableObject & _RenderableObject)
{
	*((C3DElement*)this) = (C3DElement)_RenderableObject;
	m_DebugRender = _RenderableObject.m_DebugRender;
	m_Name = _RenderableObject.m_Name;
	m_DebugRender = _RenderableObject.m_DebugRender;
	return *this;
}
