#ifndef QUATN_H
#define QUATN_H

#include "Math\MathTypes.h"
#include "Math\MathUtils.h"
#include "Math\Matrix33.h"
#include "Math\Matrix34.h"
#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include <assert.h>

template<typename T>
class Quatn
{
public:
	//------------------
	// DATOS PUBLICOS
	//------------------
	T x, y, z, w;

public:
	Quatn(){}
	Quatn(T qx, T qy,T qz,T qw) : x(qx), y(qy), z(qz), w(qw){}
	Quatn(Vector3<T> c, T r) : x(c.x), y(c.y), z(c.z), w(r){}

	//------------------------------------------------------------------------------------------------------------
	// Agregada adaptación de la librería quaternion.h de la Stanford's University Computing Science.
	//------------------------------------------------------------------------------------------------------------
	Vector3<T> GetComplex() const { return Vector3<T>(x, y, z); }
	void SetComplex(const Vector3<T>& c) { x = c.x; y = c.y; z = c.z; }

	T GetReal() const { return w; }
	void SetReal(double r) { w = r; }

	Quatn conjugate(void) const {
		return Quatn(-GetComplex(), GetReal());
	}

	/**
	* @brief Computes the inverse of this quaternion.
	*
	* @note This is a general inverse.  If you know a priori
	* that you're using a unit quaternion (i.e., norm() == 1),
	* it will be significantly faster to use conjugate() instead.
	*
	* @return The quaternion q such that q * (*this) == (*this) * q
	* == [ 0 0 0 1 ]<sup>T</sup>.
	*/
	Quatn inverse(void) const {
		return conjugate() / norm();
	}


	/**
	* @brief Computes the product of this quaternion with the
	* quaternion 'rhs'.
	*
	* @param rhs The right-hand-side of the product operation.
	*
	* @return The quaternion product (*this) x @p rhs.
	*/
	Quatn product(const Quatn& rhs) const {
		return Quatn(
			y*rhs.z - z*rhs.y + x*rhs.w + w*rhs.x,
			z*rhs.x - x*rhs.z + y*rhs.w + w*rhs.y,
			x*rhs.y - y*rhs.x + z*rhs.w + w*rhs.z,
			w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z);
	}

	/**
	* @brief Quaternion product operator.
	*
	* The result is a quaternion such that:
	*
	* result.real() = (*this).real() * rhs.real() -
	* (*this).complex().dot(rhs.complex());
	*
	* and:
	*
	* result.complex() = rhs.complex() * (*this).real
	* + (*this).complex() * rhs.real()
	* - (*this).complex().cross(rhs.complex());
	*
	* @return The quaternion product (*this) x rhs.
	*/
	Quatn operator*(const Quatn& rhs) const {
		return product(rhs);
	}

	/**
	* @brief Quaternion scalar product operator.
	* @param s A scalar by which to multiply all components
	* of this quaternion.
	* @return The quaternion (*this) * s.
	*/
	Quatn operator*(T s) const {
		return Quatn(GetComplex()*s, GetReal()*s);
	}

	/**
	* @brief Global operator allowing left-multiply by scalar.
	*/
	//Quatn operator*(double s, const Quatn& q);

	/**
	* @brief Produces the sum of this quaternion and rhs.
	*/
	Quatn operator+(const Quatn& rhs) const {
		return Quatn(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	/**
	* @brief Produces the difference of this quaternion and rhs.
	*/
	Quatn operator-(const Quatn& rhs) const {
		return Quatn(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	/**
	* @brief Unary negation.
	*/
	Quatn operator-() const {
		return Quatn(-x, -y, -z, -w);
	}

	/**
	* @brief Quaternion scalar division operator.
	* @param s A scalar by which to divide all components
	* of this quaternion.
	* @return The quaternion (*this) / s.
	*/
	Quatn operator/(T s) const {
		assert(s == 0);
		return Quatn(GetComplex() / s, GetReal() / s);
	}

	/**
	* @brief Returns a matrix representation of this
	* quaternion.
	*
	* Specifically this is the matrix such that:
	*
	* this->matrix() * q.vector() = (*this) * q for any quaternion q.
	*
	* Note that this is @e NOT the rotation matrix that may be
	* represented by a unit quaternion.
	*/
	Matrix44<T> matrix() const {
		return Matrix44<T>(
			w, -z, y, x,
			z, w, -x, y,
			-y, x, w, z,
			-x, -y, -z, w
			);
	}

	/**
	* @brief Returns a matrix representation of this
	* quaternion for right multiplication.
	*
	* Specifically this is the matrix such that:
	*
	* q.vector().transpose() * this->matrix() = (q *
	* (*this)).vector().transpose() for any quaternion q.
	*
	* Note that this is @e NOT the rotation matrix that may be
	* represented by a unit quaternion.
	*/
	Matrix44<T> rightMatrix() const {
		return Matrix44<T>(
			+w, -z, y, -x,
			+z, w, -x, -y,
			-y, x, w, -z,
			+x, y, z, w
			);
	}

	/**
	* @brief Returns this quaternion as a 4-vector.
	*
	* This is simply the vector [x y z w]<sup>T</sup>
	*/
	Vector4<T> vector() const { return Vector4<T>(x, y, z, w); }

	/**
	* @brief Returns the norm ("magnitude") of the quaternion.
	* @return The 2-norm of [ w(), x(), y(), z() ]<sup>T</sup>.
	*/
	T norm() const {
		return (T)sqrt(x*x + y*y + z*z + w*w);
	}

	/**
	* @brief Returns the norm ("magnitude") of the quaternion.
	* @return The 2-norm of [ w(), x(), y(), z() ]<sup>T</sup>.
	*/
	void normalize() {
		T norm = (T)sqrt(x*x + y*y + z*z + w*w);
		x = x / norm;
		y = y / norm;
		z = z / norm;
		w = w / norm;
	}

	/**
	* @brief Computes the rotation matrix represented by a unit
	* quaternion.
	*
	* @note This does not check that this quaternion is normalized.
	* It formulaically returns the matrix, which will not be a
	* rotation if the quaternion is non-unit.
	*/
	Matrix33<T> rotationMatrix() const {
		return Matrix33<T>(
			1 - 2 * y*y - 2 * z*z, 2 * x*y - 2 * z*w, 2 * x*z + 2 * y*w,
			2 * x*y + 2 * z*w, 1 - 2 * x*x - 2 * z*z, 2 * y*z - 2 * x*w,
			2 * x*z - 2 * y*w, 2 * y*z + 2 * x*w, 1 - 2 * x*x - 2 * y*y
			);
	}

	/**
	* @brief Returns the local up vector of the quaternion.
	*/
	Vector3<T> GetUpVector() const {
		return Vector3<T>(2 * x*y + 2 * z*w, 1 - 2 * x*x - 2 * z*z, 2 * y*z - 2 * x*w);
	}

	/**
	* @brief Returns the local right vector of the quaternion.
	*/
	Vector3<T> GetRightVector() const {
		return -Vector3<T>(1 - 2 * y*y - 2 * z*z, 2 * x*y - 2 * z*w, 2 * x*z + 2 * y*w);
	}

	/**
	* @brief Returns the local forward vector of the quaternion.
	*/
	Vector3<T> GetForwardVector() const {
		return Vector3<T>(2 * x*z - 2 * y*w, 2 * y*z + 2 * x*w, 1 - 2 * x*x - 2 * y*y);
	}

	/**
	* @brief Returns the corresponding yaw of the quaternion.
	*/
	T GetYaw() const {
		return ((T)atan2(2 * y*w - 2 * x*z, 1 - 2 * y*y - 2 * z*z));
	}

	/**
	* @brief Returns the corresponding pitch of the quaternion.
	*/
	T GetPitch() const {
		return ((T)asin(2 * x*y + 2 * z*w));
	}

	/**
	* @brief Returns the corresponding roll of the quaternion.
	*/
	T GetRoll() const {
		return (T)atan2(2*x*w - 2*y*z, 1 - 2*x*x - 2*z*z);
	}

	/**
	* @brief Returns the scaled-axis representation of this
	* quaternion rotation.
	*/
	Vector3<T> GetScaledAxis(void) const {
		Vector3<T> ret;
		T angle = 2 * (T)acos(w);
		T s = (T)sqrt(1 - w*w); // assuming quaternion normalised then w is less than 1, so term always positive.
		if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
			// if s close to zero then direction of axis not important
			ret.x = x;
			ret.y = y;
			ret.z = z;
		}
		else
		{
			ret.x = (x * angle / s);
			ret.y = (y * angle / s);
			ret.z = (z * angle / s);
		}
		return ret;
	}

	/**
	* @brief Returns the axis representation of this
	* quaternion rotation.
	*/
	Vector3<T> GetAxis(void) const {
		Vector3<T> ret;
		T s = (T)sqrt(1 - w*w); // assuming quaternion normalised then w is less than 1, so term always positive.
		if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
			// if s close to zero then direction of axis not important
			ret.x = x;
			ret.y = y;
			ret.z = z;
		}
		else
		{
			ret.x = x / s;
			ret.y = y / s;
			ret.z = z / s;
		}
		return ret;
	}

	/**
	* @brief Returns the angle of the axis representation of this
	* quaternion rotation.
	*/
	T GetAxisAngle(void) const {
		return (2 * (T)acos(w));
	}

	/**
	* @brief Sets quaternion to be same as rotation by scaled axis.
	*/
	void SetFromScaledAxis(const Vector3<T>& scaledAxis) {
		T theta = scaledAxis.Length();
		if (theta > 0.0001) {
			T s = (T)sin(theta / 2.0);
			Vector3<T> Axis(scaledAxis * s / theta);
			x = Axis[0];
			y = Axis[1];
			z = Axis[2];
			w = (T)cos(theta / 2.0);
		}
		else {
			x = y = z = 0;
			w = 1.0;
		}
	}

	/**
	* @brief Sets quaternion to be same as rotation by angle axis.
	*/
	void SetFromAngleAxis(const Vector3<T>& axis, T theta) {
		if (theta > 0.0001) {
			Vector3<T> Axis(axis * (T)sin(theta / 2.0));
			x = Axis[0];
			y = Axis[1];
			z = Axis[2];
			w = (T)cos(theta / 2.0);
		}
		else {
			x = y = z = 0;
			w = 1.0;
		}
	}

	/**
	* @brief Returns a vector rotated by this quaternion.
	*
	* Functionally equivalent to:  (rotationMatrix() * v)
	* or (q * Quaternion(0, v) * q.inverse()).
	*
	* @warning conjugate() is used instead of inverse() for better
	* performance, when this quaternion must be normalized.
	*/
	Vect3f rotatedVector(const Vector3<T>& v) const {
		return (((*this) * Quatn(v, 0)) * conjugate()).GetComplex();
	}

	/**
	* @brief Computes the quaternion from a yaw, pitch, and roll.
	*/
	void QuatFromYawPitchRoll(T yaw, T pitch, T roll)
	{
		T num = roll * 0.5f;
		T num2 = (T)sin(num);
		T num3 = (T)cos(num);
		T num4 = pitch * 0.5f;
		T num5 = (T)sin(num4);
		T num6 = (T)cos(num4);
		T num7 = yaw * 0.5f;
		T num8 = (T)sin(num7);
		T num9 = (T)cos(num7);
		x = num9 * num5 * num3 + num8 * num6 * num2;
		y = num8 * num6 * num3 - num9 * num5 * num2;
		z = num9 * num6 * num2 - num8 * num5 * num3;
		w = num9 * num6 * num3 + num8 * num5 * num2;
	}

	/**
	* @brief Computes the quaternion that is equivalent to a given
	* euler angle rotation.
	* @param euler A 3-vector in order:  roll-pitch-yaw.
	*/
	void QuatFromEuler(const Vector3<T>& euler) {
		T c1 = (T)cos(euler.z * 0.5);
		T c2 = (T)cos(euler.y * 0.5);
		T c3 = (T)cos(euler.x * 0.5);
		T s1 = (T)sin(euler.z * 0.5);
		T s2 = (T)sin(euler.y * 0.5);
		T s3 = (T)sin(euler.x * 0.5);

		x = c1*c2*s3 - s1*s2*c3;
		y = c1*s2*c3 + s1*c2*s3;
		z = s1*c2*c3 - c1*s2*s3;
		w = c1*c2*c3 + s1*s2*s3;
	}

	/** @brief Returns an equivalent euler angle representation of
	* this quaternion.
	* @return Euler angles in roll-pitch-yaw order.
	*/
	Vector3<T> EulerFromQuat(void) const {
		Vector3<T> euler;
		const static double PI_OVER_2 = DOUBLE_PI_VALUE * 0.5;
		const static double EPSILON = 1e-10;
		T sqw, sqx, sqy, sqz;

		// quick conversion to Euler angles to give tilt to user
		sqx = x*x;
		sqy = y*y;
		sqz = z*z;
		sqw = w*w;

		euler.y = (T)asin(2.0 * (w * y - x * z));
		if (PI_OVER_2 - fabs(euler.y) > EPSILON) {
			euler.z = (T)atan2(2.0 * (x * y + w * z), sqx - sqy - sqz + sqw);
			euler.x = (T)atan2(2.0 * (w * x + y * z), sqw - sqx - sqy + sqz);
		}
		else
		{
			// compute heading from local 'down' vector
			euler.z = (T)atan2(2 * y * z - 2 * x * w, 2 * x * z + 2 * y * w);
			euler.x = 0.0;

			// If facing down, reverse yaw
			if (euler.y < 0)
				euler.z = (T)(DOUBLE_PI_VALUE - euler.z);
		}
		return euler;
	}

	/**
	* @brief Computes a special representation that decouples the X
	* rotation.
	*
	* The decoupled representation is two rotations, Qyz and Qx,
	* so that Q = Qxy * Qz.
	*/
	void decoupleX(Quatn* Qyz, Quatn* Qx) const {
		Vector3<T> xtt(1, 0, 0);
		Vector3<T> xbt = this->rotatedVector(xtt);
		Vector3<T> axis_yz = xtt^xbt;
		T axis_norm = axis_yz.Length();

		T axis_theta = acos(mathUtils::Clamp(xbt.z, -1.f, +1.f));
		if (axis_norm > 0.00001) {
			axis_yz = axis_yz * (axis_theta / axis_norm); // limit is *1
		}

		Qyz->SetFromScaledAxis(axis_yz);
		*Qx = (Qyz->conjugate() * (*this));
	}

	/**
	* @brief Computes a special representation that decouples the Y
	* rotation.
	*
	* The decoupled representation is two rotations, Qxz and Qy,
	* so that Q = Qxy * Qz.
	*/
	void decoupleY(Quatn* Qxz, Quatn* Qy) const {
		Vector3<T> ytt(0, 1, 0);
		Vector3<T> ybt = this->rotatedVector(ytt);
		Vector3<T> axis_xz = ytt^ybt;
		T axis_norm = axis_xz.Length();

		T axis_theta = acos(mathUtils::Clamp(ybt.y, -1.f, +1.f));
		if (axis_norm > 0.00001) {
			axis_xz = axis_xz * (axis_theta / axis_norm); // limit is *1
		}

		Qxz->SetFromScaledAxis(axis_xz);
		*Qy = (Qxz->conjugate() * (*this));
	}

	/**
	* @brief Computes a special representation that decouples the Z
	* rotation.
	*
	* The decoupled representation is two rotations, Qxy and Qz,
	* so that Q = Qxy * Qz.
	*/
	void decoupleZ(Quatn* Qxy, Quatn* Qz) const {
		Vector3<T> ztt(0, 0, 1);
		Vector3<T> zbt = this->rotatedVector(ztt);
		Vector3<T> axis_xy = ztt^zbt;
		T axis_norm = axis_xy.Length();

		T axis_theta = acos(mathUtils::Clamp(zbt.z, -1.f, +1.f));
		if (axis_norm > 0.00001) {
			axis_xy = axis_xy * (axis_theta / axis_norm); // limit is *1
		}

		Qxy->SetFromScaledAxis(axis_xy);
		*Qz = (Qxy->conjugate() * (*this));
	}

	/**
	* @brief Returns the quaternion slerped between this and q1 by fraction 0 <= t <= 1.
	*/
	Quatn slerp(const Quatn& q1, T t) {
		return slerp(*this, q1, t);
	}

	/// Returns quaternion that is slerped by fraction 't' between q0 and q1.
	static Quatn slerp(const Quatn& q0, const Quatn& q1, T t) {

		T omega = (T)acos(mathUtils::Clamp(
			q0.x * q1.x +
			q0.y * q1.y +
			q0.z * q1.z +
			q0.w * q1.w,
			-1.f, 1.f));
		if (fabs(omega) < 1e-10) {
			omega = (T)1e-10;
		}
		T som = (T)sin(omega);
		T st0 = (T)sin((1 - t) * omega) / som;
		T st1 = (T)sin(t * omega) / som;

		return Quatn(
			q0.x * st0 + q1.x * st1,
			q0.y * st0 + q1.y * st1,
			q0.z * st0 + q1.z * st1,
			q0.w * st0 + q1.w * st1
			);
	}

	/**
	* @brief Returns the quaternion slerped between this and q1 by fraction 0 <= t <= 1.
	*/
	bool Compare(const Quatn& q1) {
		if (x == q1.x && y == q1.y && z == q1.z && w == q1.w)
			return true;
		else
			return false;
	}

	Vector3<T> RotateVectorByQuat(const Vector3<T>& v)
	{
		Vector3<T> vprime;

		// Extract the vector part of the quaternion
		Vector3<T> u(x, y, z);

		// Extract the scalar part of the quaternion
		T s = w;

		// Do the math
		vprime = 2.0f * (u * v) * u
			+ (s*s - (u * u)) * v
			+ 2.0f * s * (u ^ v);

		return vprime;
	}

	static Vector3<T> rotate_vector_by_quaternion(const Vector3<T>& v, const Quatn& q)
	{
		Vector3<T> vprime;

		// Extract the vector part of the quaternion
		Vector3<T> u(q.x, q.y, q.z);

		// Extract the scalar part of the quaternion
		T s = q.w;

		// Do the math
		vprime = 2.0f * (u * v) * u
			+ (s*s - (u * u)) * v
			+ 2.0f * s * (u ^ v);

		return vprime;
	}

	/**
	* @brief Sets quaternion to be _fwd axis with _up angle.
	*/
	void SetFromFwdUp(const Vector3<T>& _fwd, const Vector3<T>& _up) {
		Vector3<T> l_V1 = _fwd.GetNormalized();
		Vector3<T> l_V2 = (_up^l_V1).GetNormalized();
		Vector3<T> l_V3 = l_V1^l_V2;

		T m00 = l_V2.x;
		T m10 = l_V2.y;
		T m20 = l_V2.z;

		T m01 = l_V3.x;
		T m11 = l_V3.y;
		T m21 = l_V3.z;

		T m02 = l_V1.x;
		T m12 = l_V1.y;
		T m22 = l_V1.z;

		T num8 = (m00 + m11) + m22;

		if (num8 > 0.0)
		{
			T num = (T)sqrt(num8 + (T)1.0);
			w = num * (T)0.5;
			num = (T)0.5 / num;
			x = (m12 - m21) * num;
			y = (m20 - m02) * num;
			z = (m01 - m10) * num;
		}
		else if ((m00 >= m11) && (m00 >= m22))
		{
			T num7 = (T)sqrt((((T)1.0 + m00) - m11) - m22);
			T num4 = 0.5 / num7;
			x = (T)0.5 * num7;
			y = (m01 + m10) * num4;
			z = (m02 + m20) * num4;
			w = (m12 - m21) * num4;
		}
		else if (m11 > m22)
		{
			T num6 = (T)sqrt((((T)1.0 + m11) - m00) - m22);
			T num3 = (T)0.5 / num6;
			x = (m10 + m01) * num3;
			y = (T)0.5 * num6;
			z = (m21 + m12) * num3;
			w = (m20 - m02) * num3;
		}
		else
		{
			T num5 = (T)sqrt((((T)1.0 + m22) - m00) - m11);
			T num2 = (T)0.5 / num5;
			x = (m20 + m02) * num2;
			y = (m21 + m12) * num2;
			z = (T)0.5 * num5;
			w = (m01 - m10) * num2;
		}
	}
};

typedef Quatn<float> Quatf;
typedef Quatn<double> Quatd;
typedef Quatn<int32_t>  Quati;
typedef Quatn<uint32_t> Quatu;
typedef Quatn<uint8_t>  Quatu8;
typedef Quatn<uint16_t> Quatw;

extern const Quatf qfIDENTITY;
extern const Quatf qfINVALID;

extern const Quatd qdIDENTITY;
extern const Quatd qdINVALID;

extern const Transformf quatposfIDENTITY;  
extern const Transformd quatposdIDENTITY;

#endif //QUATN_H
