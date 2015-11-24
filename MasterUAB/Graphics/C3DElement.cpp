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
{}

C3DElement::~C3DElement(void)
{
}

void C3DElement::Render(CKGRenderManager *RenderManager)
{
	assert(!"This method mustn't be called");
}