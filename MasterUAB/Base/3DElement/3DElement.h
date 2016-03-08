#ifndef C3D_ELEMENT_H
#define C3D_ELEMENT_H

#include "Math\Matrix44.h"
#include <d3d11.h>

class CXMLTreeNode;
class CEmptyPointerClass;
class CRenderManager;

class C3DElement
{
protected:
	Vect3f m_Position, m_PrevPos;
	float m_Yaw, m_Pitch, m_Roll;
	bool m_TranslationUpdate, m_RotationUpdate, m_ScaleUpdate;
	Mat44f m_TransformMatrix, m_RotationMatrix, m_TranslationMatrix,
	m_ScaleMatrix;
	Vect3f m_Scale;
	bool m_Visible;
public:
	C3DElement();
	C3DElement(const Vect3f &Position);
	C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll);
	C3DElement(float Yaw, float Pitch, float Roll);
	C3DElement(const CXMLTreeNode &XMLTreeNode);
	virtual ~C3DElement(void);
	virtual void SetPosition(const Vect3f &Position);
	inline const Vect3f & GetPosition() const{
		return m_Position;
	}
	inline const Vect3f & GetPrevPosition() const
	{
		return m_PrevPos;
	}
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;
	virtual void SetYaw(float Yaw);
	virtual void SetPitch(float Pitch);
	virtual void SetRoll(float Roll);
	virtual void SetYawPitchRoll(float Yaw, float Pitch, float Roll);
	virtual void SetScale(const Vect3f &Scale);
	inline const Vect3f & GetScale() const
	{
		return m_Scale;
	}
	virtual void Render(CRenderManager *RenderManager);
	const Mat44f & GetTransform();
	float GetVisible() const;
	virtual void SetVisible(bool newVisible);

	virtual CEmptyPointerClass*  GetPositionX();
	virtual CEmptyPointerClass*  GetPositionY();
	virtual CEmptyPointerClass*  GetPositionZ();
};

#endif //C3D_ELEMENT_H