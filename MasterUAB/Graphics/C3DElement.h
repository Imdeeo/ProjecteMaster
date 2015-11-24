#ifndef C3D_ELEMENT_H
#define C3D_ELEMENT_H

#include "Math\Matrix44.h"
#include <d3d11.h>
#include "XML\XMLTreeNode.h"

class CKGRenderManager;

class C3DElement
{
protected:
	Vect3f m_Position, m_PrevPos;
	float m_Yaw, m_Pitch, m_Roll;
	bool m_TranslationUpdate, m_RotationUpdate, m_ScaleUpdate;
	Mat44f m_TransformMatrix, m_RotationMatrix, m_TranslationMatrix,
	m_ScaleMatrix;
	Vect3f m_Scale;
public:
	C3DElement();
	C3DElement(const Vect3f &Position);
	C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll);
	C3DElement(float Yaw, float Pitch, float Roll);
	C3DElement(const CXMLTreeNode &XMLTreeNode);
	virtual ~C3DElement(void);
	virtual void SetPosition(const Vect3f &Position)
	{
		m_PrevPos = m_Position;
		m_Position = Position;
	}
	inline const Vect3f & GetPosition() const
	{
		return m_Position;
	}
	inline const Vect3f & GetPrevPosition() const
	{
		return m_PrevPos;
	}
	float GetYaw() const
	{
		return m_Yaw;
	}
	float GetPitch() const
	{
		return m_Pitch;
	}
	float GetRoll() const
	{
		return m_Roll;
	}
	virtual void SetYaw(float Yaw)
	{
		m_Yaw = Yaw;
	}
	virtual void SetPitch(float Pitch)
	{
		m_Pitch = Pitch;
	}
	virtual void SetRoll(float Roll)
	{
		m_Roll = Roll;
	}
	virtual void SetYawPitchRoll(float Yaw, float Pitch, float Roll)
	{
		m_Yaw = Yaw;
		m_Pitch = Pitch;
		m_Roll = Roll;
	}
	virtual void SetScale(const Vect3f &Scale)
	{
		m_Scale = Scale;
	}
	inline const Vect3f & GetScale() const
	{
		return m_Scale;
	}
	virtual void Render(CKGRenderManager *RenderManager);
	const Mat44f & GetTransform()
	{
		return m_TransformMatrix;
	}
};

#endif //C3D_ELEMENT_H