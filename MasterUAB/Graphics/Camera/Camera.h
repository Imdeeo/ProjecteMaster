#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include <string>

class CCamera
{
private:
	Mat44f				m_View;
	Mat44f				m_Projection;

	Vect3f					m_Position;
	Vect3f					m_LookAt;
	Vect3f					m_Up;

	float						m_FOV;
	float						m_AspectRatio;
	float						m_ZNear;
	float						m_ZFar;

public:
	enum TCameraType
	{
		CAMERA_TYPE_SPHERICAL = 0,
		CAMERA_TYPE_FPS = 1,
		CAMERA_TYPE_CYCLE = 2,
		CAMERA_TYPE_REVERSE = 4,

		CAMERA_TYPE_NULL = -1
	};

	CCamera()
		: m_FOV(/*60.0f*/ 1.047198f)
		, m_AspectRatio(1.0f)
		, m_ZNear(0.1f)
		, m_ZFar(100.0f)
		, m_Position(5.0f, 5.0f, 0.0f)
		, m_LookAt(0.0f, 0.0f, 0.0f)
		, m_Up(0.0f, 1.0f, 0.0f)
	{}
	virtual ~CCamera() {}

	void SetFOV(float FOV) {m_FOV=FOV;}
	float GetFOV() const {return m_FOV;}
	void SetAspectRatio(float AspectRatio) {m_AspectRatio=AspectRatio;}
	float GetAspectRatio() const {return m_AspectRatio;}
	void SetZNear(float ZNear) {m_ZNear=ZNear;}
	float GetZNear() const {return m_ZNear;}
	void SetZFar(float ZFar) {m_ZFar=ZFar;}
	float GetZFar() const {return m_ZFar;}

	void SetPosition(const Vect3f &Position)
	{
		m_Position=Position;
	}
	const Vect3f & GetPosition() const
	{
		return m_Position;
	}
	void SetLookAt(const Vect3f &LookAt)
	{
		m_LookAt=LookAt;
	}
	const Vect3f & GetLookAt() const
	{
		return m_LookAt;
	}
	void SetUp(const Vect3f &Up)
	{
		m_Up=Up;
	}
	const Vect3f & GetUp() const
	{
		return m_Up;
	}


	const Mat44f & GetView() const { return m_View; }
	const Mat44f & GetProjection() const { return m_Projection; }

	void SetMatrixs()
	{
		m_View.SetIdentity();
		m_View.SetFromLookAt(m_Position, m_LookAt, m_Up);

		m_Projection.SetIdentity();
		m_Projection.SetFromPerspective(m_FOV, m_AspectRatio, m_ZNear, m_ZFar);
	}

	static CCamera::TCameraType CCamera::GetCameraTypeByName(const std::string &CameraType)
	{
		if(CameraType=="spherical")
			return CAMERA_TYPE_SPHERICAL;
		else if (CameraType=="fps")
			return CAMERA_TYPE_FPS;
		else if (CameraType=="cycle")
			return CAMERA_TYPE_CYCLE;
		else if (CameraType=="reverse")
			return CAMERA_TYPE_REVERSE;
		return CAMERA_TYPE_NULL;
	}
};

#endif
