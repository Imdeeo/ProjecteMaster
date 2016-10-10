#ifndef C3D_ELEMENT_H
#define C3D_ELEMENT_H

#include "Math\Matrix44.h"
#include "Math\Quatn.h"
#include "no_sillywarnings_please.h"
#include "XML\tinyxml2.h"
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
	Vect3f m_Scale;
	Vect3f m_AnimatedPosition;
	Quatf m_AnimatedRotation;
	Vect3f m_AnimatedScale;
	bool m_TranslationUpdate;
	bool m_RotationUpdate;
	bool m_ScaleUpdate;
	Mat44f m_TransformMatrix;
	Mat44f m_RotationMatrix;
	Mat44f m_TranslationMatrix;
	Mat44f m_ScaleMatrix;
	Mat44f m_AnimatedScaleMatrix;
	Mat44f m_AnimatedRotationMatrix;
	Mat44f m_AnimatedTranslationMatrix;
	bool m_Visible;
	bool m_IsCinematic;
public:
	C3DElement();
	C3DElement(const Vect3f &Position);
	C3DElement(const Vect3f &Position, const Quatf &Rotation);
	C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll);
	C3DElement(const Quatf &Rotation);
	C3DElement(float Yaw, float Pitch, float Roll);
	C3DElement(tinyxml2::XMLElement* TreeNode);
	C3DElement(const CXMLTreeNode &TreeNode);
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
	virtual void SetAnimatedPosition(const Vect3f &Position);
	inline const Vect3f & GetAnimatedPosition() const{
		return m_AnimatedPosition;
	}
	Quatf GetAnimatedRotation() const{ return m_AnimatedRotation; }
	virtual void SetAnimatedRotation(Quatf Rotation){ m_AnimatedRotation = Rotation; }
	virtual void SetAnimatedScale(const Vect3f &Scale);
	inline const Vect3f & GetAnimatedScale() const
	{
		return m_AnimatedScale;
	}
	virtual void Render(CRenderManager *RenderManager);
	const Mat44f & GetTransform();
	bool GetVisible() const;
	virtual void SetVisible(bool newVisible);

	virtual CEmptyPointerClass*  GetPositionX();
	virtual CEmptyPointerClass*  GetPositionY();
	virtual CEmptyPointerClass*  GetPositionZ();

	void SetIsCinematic(bool _IsCinematic)
	{
		m_IsCinematic = _IsCinematic;
	}

	void ResetAnimatedValues();
};

#endif //C3D_ELEMENT_H