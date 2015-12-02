#include "C3DElement.h"
#include <assert.h>


C3DElement::C3DElement(void)
{
}

C3DElement::C3DElement(const Vect3f &Position):
	m_Position(Position)
{
}
C3DElement::C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll):
	m_Position(Position),
	m_Yaw(Yaw),
	m_Pitch(Pitch),
	m_Roll(Roll)
{
}
C3DElement::C3DElement(float Yaw, float Pitch, float Roll):
	m_Yaw(Yaw),
	m_Pitch(Pitch),
	m_Roll(Roll)
{}
C3DElement::C3DElement(const CXMLTreeNode &XMLTreeNode)
{
	CXMLTreeNode l_Element = XMLTreeNode; 
	m_Position = l_Element.GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f), true);
	m_Yaw = l_Element.GetFloatProperty("yaw",0.f,true);
	m_Pitch = l_Element.GetFloatProperty("pitch",0.f,true);
	m_Roll = l_Element.GetFloatProperty("roll",0.f,true);
	m_Scale = l_Element.GetFloatProperty("scale",1.f,true);
	m_Visible = l_Element.GetBoolProperty("visible",false,true);
}

C3DElement::~C3DElement(void)
{
}

void C3DElement::Render(CKGRenderManager *RenderManager)
{
	assert(!"This method mustn't be called");
}