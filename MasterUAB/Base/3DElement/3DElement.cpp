#include "3DElement\3DElement.h"
#include "XML\XMLTreeNode.h"
#include "Utils\CEmptyPointerClass.h"
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
	const char * existPos = l_Element.GetPszProperty("position");
	if (existPos == NULL)
	{
		m_Position = l_Element.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f), true);
	}
	else
	{
		m_Position = l_Element.GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f), true);
	}
	m_Yaw = l_Element.GetFloatProperty("yaw",0.f,true);
	m_Pitch = l_Element.GetFloatProperty("pitch",0.f,true);
	m_Roll = l_Element.GetFloatProperty("roll",0.f,true);
	const char * existScale = l_Element.GetPszProperty("scale");
	if (existScale == NULL)
	{
		m_Scale = Vect3f(1.f, 1.f, 1.f);
	}
	else
	{
		m_Scale = l_Element.GetVect3fProperty("scale", Vect3f(1.f, 1.f, 1.f), true);
		m_Scale.x = abs(m_Scale.x);
	}
	m_Visible = l_Element.GetBoolProperty("visible", true, false);	
}

C3DElement::~C3DElement(void)
{
}

void C3DElement::Render(CRenderManager *RenderManager)
{
	assert(!"This method mustn't be called");
}


const Mat44f & C3DElement::GetTransform()
{
	m_ScaleMatrix.SetIdentity();
	m_ScaleMatrix.Scale(m_Scale.x, m_Scale.y, m_Scale.z);
	
	m_RotationMatrix.SetIdentity();

	/* Using m_RotationMatrix.SetPitchRollYaw has problems with
	multiplication order,so it's better to do rotations separately. */
	Mat44f l_RotX;
	l_RotX.SetIdentity();
	l_RotX.RotByAngleX(m_Roll);
	Mat44f l_RotY;
	l_RotY.SetIdentity();
	l_RotY.RotByAngleY(m_Yaw);
	Mat44f l_RotZ;
	l_RotZ.SetIdentity();
	l_RotZ.RotByAngleZ(m_Pitch);

	m_RotationMatrix=l_RotX*l_RotZ*l_RotY;

	m_TranslationMatrix.SetIdentity();
	m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

	m_TransformMatrix=m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;
	
	return m_TransformMatrix;
}

void C3DElement::SetPosition(const Vect3f &Position)
{
	m_PrevPos = m_Position;
	m_Position = Position;
}
float C3DElement::GetYaw() const
{
	return m_Yaw;
}
float C3DElement::GetPitch() const
{
	return m_Pitch;
}
float C3DElement::GetRoll() const
{
	return m_Roll;
}
void C3DElement::SetYaw(float Yaw)
{
	m_Yaw = Yaw;
}
void C3DElement::SetPitch(float Pitch)
{
	m_Pitch = Pitch;
}
void C3DElement::SetRoll(float Roll)
{
	m_Roll = Roll;
}
void C3DElement::SetYawPitchRoll(float Yaw, float Pitch, float Roll)
{
	m_Yaw = Yaw;
	m_Pitch = Pitch;
	m_Roll = Roll;
}
void C3DElement::SetScale(const Vect3f &Scale)
{
	m_Scale = Scale;
}

float C3DElement::GetVisible() const
{
	return m_Visible;
}

void C3DElement::SetVisible(bool newVisible)
{
	m_Visible = newVisible;
}

CEmptyPointerClass*  C3DElement::GetPositionX()
{
	return (CEmptyPointerClass*)(&m_Position.x);
}
CEmptyPointerClass*  C3DElement::GetPositionY()
{
	return (CEmptyPointerClass*)(&m_Position.y);
}
CEmptyPointerClass*  C3DElement::GetPositionZ()
{
	return (CEmptyPointerClass*)(&m_Position.z);
}