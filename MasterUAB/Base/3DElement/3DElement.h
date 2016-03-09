#ifndef C3D_ELEMENT_H
#define C3D_ELEMENT_H

#include "Math\Matrix44.h"
#include "Math\Quatn.h"
#include <d3d11.h>

class CXMLTreeNode;
class CEmptyPointerClass;
class CRenderManager;

class C3DElement
{
protected:
	Vect3f m_Position;
	Vect3f m_PrevPos;
	Quatf m_Rotation;
	bool m_TranslationUpdate;
	bool m_RotationUpdate;
	bool m_ScaleUpdate;
	Mat44f m_TransformMatrix;
	Mat44f m_TranslationRotationMatrix;
	Mat44f m_ScaleMatrix;
	Vect3f m_Scale;
	bool m_Visible;
public:
	C3DElement();
	C3DElement(const Vect3f &Position);
	C3DElement(const Vect3f &Position, const Quatf &Rotation);
	C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll);
	C3DElement(const Quatf &Rotation);
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
	Quatf GetRotation() const{ return m_Rotation; }
	virtual void SetRotation(Quatf Rotation){ m_Rotation = Rotation; }
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