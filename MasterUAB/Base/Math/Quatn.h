#ifndef QUATN_H
#define QUATN_H

#include "Math\MathTypes.h"
#include "Math\MathUtils.h"
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
	Quatn(Vect3f c, T r) : x(c.x), y(c.y), z(c.z), w(r){}

	//------------------------------------------------------------------------------------------------------------
	// Agregada adaptación de la librería quaternion.h de la Stanford's University Computing Science.
	//------------------------------------------------------------------------------------------------------------
	Vect3f GetComplex() const { return Vect3f(x, y, z); }
	void SetComplex(const Vect3f& c) { x = c.x; y = c.y; z = c.z; }

	double GetReal() const { return w; }
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
	Quatn operator*(double s) const {
		return Quatn(GetComplex()*s, GetReal()*s);
	}

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
	Quatn operator/(double s) const {
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
	Mat44f matrix() const {
		return Mat44f(
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
	Mat44f rightMatrix() const {
		return Mat44f(
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
	Vect4f vector() const { return Vect4f(x, y, z, w); }

	/**
	* @brief Returns the norm ("magnitude") of the quaternion.
	* @return The 2-norm of [ w(), x(), y(), z() ]<sup>T</sup>.
	*/
	double norm() const {
		return sqrt(x*x + y*y + z*z + w*w);
	}

	/**
	* @brief Computes the rotation matrix represented by a unit
	* quaternion.
	*
	* @note This does not check that this quaternion is normalized.
	* It formulaically returns the matrix, which will not be a
	* rotation if the quaternion is non-unit.
	*/
	Mat33f rotationMatrix() const {
		return Mat33f(
			1 - 2 * y*y - 2 * z*z, 2 * x*y - 2 * z*w, 2 * x*z + 2 * y*w,
			2 * x*y + 2 * z*w, 1 - 2 * x*x - 2 * z*z, 2 * y*z - 2 * x*w,
			2 * x*z - 2 * y*w, 2 * y*z + 2 * x*w, 1 - 2 * x*x - 2 * y*y
			);
	}

	/**
	* @brief Returns the scaled-axis representation of this
	* quaternion rotation.
	*/
	/*Vect3f GetScaledAxis(void) const {
		// Euclideanspace.com code
		Vect3f ret;
		if (w > 1)
			norm();
		angle = 2 * acos(w);
		double s = sqrt(1 - w*w); // assuming quaternion normalised then w is less than 1, so term always positive.
		if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
			// if s close to zero then direction of axis not important
			ret.x = x; // if it is important that axis is normalised then replace with x=1; y=z=0;
			ret.y = y;
			ret.z = z;
		}
		else
		{
			ret.x = x / s; // normalise axis
			ret.y = y / s;
			ret.z = z / s;
		}
		return ret;

		// Stanford code (need HeliMath)
		double w[3];
		HeliMath::scaled_axis_from_quaternion(w, mData);
		return Vect3f(w);
	}*/

	/**
	* @brief Sets quaternion to be same as rotation by scaled axis w.
	*/
	void SetScaledAxis(const Vect3f& w) {
		double theta = w.Normalize();
		if (theta > 0.0001) {
			double s = sin(theta / 2.0);
			Vect3f W(w / theta * s);
			x = W[0];
			y = W[1];
			z = W[2];
			w = cos(theta / 2.0);
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
	Vect3f rotatedVector(const Vect3f& v) const {
		return (((*this) * Quatn(v, 0)) * conjugate()).GetComplex();
	}



	/**
	* @brief Computes the quaternion that is equivalent to a given
	* euler angle rotation.
	* @param euler A 3-vector in order:  roll-pitch-yaw.
	*/
	void QuatFromEuler(const Vect3f& euler) {
		double c1 = cos(euler.z * 0.5);
		double c2 = cos(euler.y * 0.5);
		double c3 = cos(euler.x * 0.5);
		double s1 = sin(euler.z * 0.5);
		double s2 = sin(euler.y * 0.5);
		double s3 = sin(euler.x * 0.5);

		x = c1*c2*s3 - s1*s2*c3;
		y = c1*s2*c3 + s1*c2*s3;
		z = s1*c2*c3 - c1*s2*s3;
		w = c1*c2*c3 + s1*s2*s3;
	}

	/** @brief Returns an equivalent euler angle representation of
	* this quaternion.
	* @return Euler angles in roll-pitch-yaw order.
	*/
	Vect3f EulerFromQuat(void) const {
		Vect3f euler;
		const static double PI_OVER_2 = M_PI * 0.5;
		const static double EPSILON = 1e-10;
		double sqw, sqx, sqy, sqz;

		// quick conversion to Euler angles to give tilt to user
		sqx = x*x;
		sqy = y*y;
		sqz = z*z;
		sqw = w*w;

		euler.y = asin(2.0 * (w * y - x * z));
		if (PI_OVER_2 - fabs(euler.y) > EPSILON) {
			euler.z = atan2(2.0 * (x * y + w * z), sqx - sqy - sqz + sqw);
			euler.x = atan2(2.0 * (w * x + y * z), sqw - sqx - sqy + sqz);
		}
		else
		{
			// compute heading from local 'down' vector
			euler.z = atan2(2 * y * z - 2 * x * w, 2 * x * z + 2 *y * w);
			euler.x = 0.0;

			// If facing down, reverse yaw
			if (euler.y < 0)
				euler.z = M_PI - euler.z;
		}
		return euler;
	}

	/**
	* @brief Computes a special representation that decouples the Z
	* rotation.
	*
	* The decoupled representation is two rotations, Qxy and Qz,
	* so that Q = Qxy * Qz.
	*/
	/*void decoupleZ(Quatn* Qxy, Quatn* Qz) const {
		Vect3f ztt(0, 0, 1);
		Vect3f zbt = this->rotatedVector(ztt);
		Vect3f axis_xy = ztt^zbt;
		double axis_norm = axis_xy.Normalize();
		
		double axis_theta = acos(mathUtils::Clamp(zbt.z, -1, +1));
		if (axis_norm > 0.00001) {
			axis_xy = axis_xy * (axis_theta / axis_norm); // limit is *1
		}

		Qxy->GetScaledAxis(axis_xy);
		*Qz = (Qxy->conjugate() * (*this));
	}*/

	/**
	* @brief Returns the quaternion slerped between this and q1 by fraction 0 <= t <= 1.
	*/
	Quatn slerp(const Quatn& q1, double t) {
		return slerp(*this, q1, t);
	}

	/// Returns quaternion that is slerped by fraction 't' between q0 and q1.
	static Quatn slerp(const Quatn& q0, const Quatn& q1, double t) {

		double omega = acos(mathUtils::Clamp(
			q0.x * q1.x +
			q0.y * q1.y +
			q0.z * q1.z +
			q0.w * q1.w,
			-1, 1));
		if (fabs(omega) < 1e-10) {
			omega = 1e-10;
		}
		double som = sin(omega);
		double st0 = sin((1 - t) * omega) / som;
		double st1 = sin(t * omega) / som;

		return Quatn(
			q0.x * st0 + q1.x * st1,
			q0.y * st0 + q1.y * st1,
			q0.z * st0 + q1.z * st1,
			q0.w * st0 + q1.w * st1
			);
	}

	/**
	* @brief Returns pointer to the internal array.
	*
	* Array is in order x,y,z,w.
	*/
	//double* row(uint32_t i) { return mData + i; }
	// Const version of the above.
	//const double* row(uint32_t i) const { return mData + i; }

	/**
	* @brief Global operator allowing left-multiply by scalar.
	*/
	Quatn operator*(double s, const Quatn& q);
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
