#ifndef INC_CAMERACONTROLLER_H_
#define INC_CAMERACONTROLLER_H_

#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include "Utils\Named.h"
#include "Math\Quatn.h"

class CCamera;
class CXMLTreeNode;

class CCameraController : public CNamed
{
protected:
	Vect3f m_Position;
	Quatf m_Rotation;
public:
	CCameraController(const CXMLTreeNode &);
	virtual ~CCameraController();
	virtual void SetCamera(CCamera *Camera) const = 0;
	Vect3f GetDirection() const;
	virtual void AddYaw(float Radians);
	virtual void AddPitch(float Radians);
	
	void SetRotation(Quatf Rotation) { m_Rotation = Rotation; }
	Quatf GetRotation() const { return m_Rotation; }
	
	Vect3f GetRight() const;
	Vect3f GetUp() const;
	Vect3f GetForward() const;

	const Vect3f & GetPosition() const { return m_Position; }
	void SetPosition(const Vect3f &Position) { m_Position = Position; }

	virtual void Update(float ElapsedTime)
	{
	}
};

#endif
