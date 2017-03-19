#include "3DElement\3DElement.h"
#include "Utils\CEmptyPointerClass.h"
#include <assert.h>


C3DElement::C3DElement(void):
	m_Position(Vect3f(0, 0, 0)),
	m_Rotation(Quatf(0, 0, 0, 1)),
	m_IsCinematic(false)
{
}

C3DElement::C3DElement(const Vect3f &Position):
	m_Position(Position),
	m_Rotation(Quatf(0, 0, 0, 1)),
	m_IsCinematic(false)
{
}

C3DElement::C3DElement(const Vect3f &Position, const Quatf &Rotation) :
	m_Position(Position),
	m_Rotation(Rotation),
	m_IsCinematic(false)
{
}

C3DElement::C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll):
	m_Position(Position),
	m_Rotation(Quatf(0, 0, 0, 1)),
	m_IsCinematic(false)
{
	m_Rotation.QuatFromEuler(Vect3f(Yaw, Pitch, Roll));
}

C3DElement::C3DElement(const Quatf &Rotation) :
	m_Position(Vect3f(0, 0, 0)),
	m_Rotation(Rotation),
	m_IsCinematic(false)
{
}

C3DElement::C3DElement(float Yaw, float Pitch, float Roll):
	m_Position(Vect3f(0, 0, 0)),
	m_Rotation(Quatf(0, 0, 0, 1)),
	m_IsCinematic(false)
{
	m_Rotation.QuatFromEuler(Vect3f(Yaw, Pitch, Roll));
}

C3DElement::C3DElement(tinyxml2::XMLElement* l_Element) :
	m_Position(Vect3f(0, 0, 0)),
	m_Rotation(Quatf(0, 0, 0, 1)),
	m_IsCinematic(false)
{
	 
	const char * existPos = l_Element->GetPszProperty("position");
	if (existPos == "")
	{
		m_Position = l_Element->GetVect3fProperty("pos", Vect3f(.0f, 0.0f, .0f));
	}
	else
	{
		m_Position = l_Element->GetVect3fProperty("position", Vect3f(.0f, .0f, .0f));
	}
	const char * existRotation = l_Element->GetPszProperty("rotation");
	if (existRotation == "")
	{		
		m_Rotation.QuatFromYawPitchRoll(
			l_Element->GetFloatProperty("yaw", .0f),
			l_Element->GetFloatProperty("pitch", .0f),
			l_Element->GetFloatProperty("roll", .0f));

	} else
	{
		m_Rotation = l_Element->GetQuatfProperty("rotation", Quatf(.0f, .0f, .0f, 1.f));
	}
	m_Rotation.normalize();
	
	const char * existScale = l_Element->GetPszProperty("scale");
	if (existScale == NULL)
	{
		m_Scale = Vect3f(1.f, 1.f, 1.f);
	}
	else
	{
		m_Scale = l_Element->GetVect3fProperty("scale", Vect3f(1.f, 1.f, 1.f));
		m_Scale.x = abs(m_Scale.x);
	}
	m_Visible = l_Element->GetBoolProperty("visible", true);	
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
	if (m_IsCinematic)
	{
		return m_TransformMatrix;
	}
	m_ScaleMatrix.SetIdentity();
	m_ScaleMatrix.Scale(m_Scale.x, m_Scale.y, m_Scale.z);

	m_RotationMatrix.SetIdentity();
	m_RotationMatrix = m_Rotation.rotationMatrix();

	m_TranslationMatrix.SetIdentity();
	m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

	m_TransformMatrix = m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;
	
	return m_TransformMatrix;
}

void C3DElement::SetPosition(const Vect3f &Position)
{
	m_PrevPos = m_Position;
	m_Position = Position;
}

void C3DElement::SetScale(const Vect3f &Scale)
{
	m_Scale = Scale;
}

void C3DElement::SetAnimatedPosition(const Vect3f &Position)
{
	m_AnimatedPosition = Position;
}

void C3DElement::SetAnimatedScale(const Vect3f &Scale)
{
	m_AnimatedScale = Scale;
}

bool C3DElement::GetVisible() const
{
	return m_Visible;
}

void C3DElement::SetVisible(bool newVisible)
{
	m_Visible = newVisible;
}

void C3DElement::ResetAnimatedValues()
{
	m_AnimatedPosition = Vect3f(0.0f, 0.0f, 0.0f);
	m_AnimatedRotation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
	m_AnimatedScale = Vect3f(1.0f, 1.0f, 1.0f);
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