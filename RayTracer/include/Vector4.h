#pragma once

#include <cstdint>
#include <cmath>
#include <ostream>

#include "Vector3.h"

template <typename T>
/**
* Class of manipulating a 4 component vector.
*/
class Vector4
{
public:
	/**
	* Constructs vector with x, y, z, w components.
	* @param X - value of X component
	* @param Y - value of Y component
	* @param Z - value of Z component
	* @param W - value of W component
	*/
	Vector4(T X = 0, T Y = 0, T Z = 0, T W = 1);

	/**
	* Constructs vector with first 3 components of
	* Vector3.
	* @param Vec3 Vector with x, y, z components.
	*/
	Vector4(const Vector3<T>& Vec3);

	template <typename U>
	/**
	* Performs vector-scalar multiplication.
	* @param scalar - Scalar to multiply by.
	* @return Reference to this vector.
	*/
	Vector4<T>& operator*=(const U& scalar);

	/**
	* Performs vector addition.
	* @param rhs - Vector4 to add.
	* @return Reference to this vector.
	*/
	Vector4<T>& operator+=(const Vector4<T>& rhs);

	/**
	* Performs vector subtraction.
	* @param rhs - Vector4 to subtract
	* @return Reference to this vector
	*/
	Vector4<T>& operator-=(const Vector4<T>& rhs);

	template <typename U>
	/**
	* Performs vector division.
	* @param rhs - Unit to divide by.
	* @return Reference to this vector
	*/
	Vector4<T>& operator/=(const U& scalar);

	/**
	* Overload of subscript operators for X = 0, Y = 1, Z = 2, W = 3.
	*/
	T& operator[](std::size_t idx);

	/**
	* Overload of subscript operators for X = 0, Y = 1, Z = 2, W = 3..
	*/
	const T& operator[](std::size_t idx) const;

	/**
	* Normalizes x, y, z components of the vector.
	*/
	Vector4<T>& Normalize3();

	/**
	* Calculates the length/magnatude of the vector with all four
	* components.
	* @return The length/magnatude
	*/
	float Length4() const;

	/**
	* Calculates the length/magnatude of the vector with x, y, z
	* components.
	* @return The length/magnatude
	*/
	float Length3() const;

	/**
	* Reflects the vector against a normal.
	* @param Normal the normal being reflected by
	* @return Reflected vector
	*/
	Vector4<T> Reflect3(const Vector4<T>& Normal) const;

	/**
	* Prints the string representation of the vector.
	*/
	void Print() const;

	/**
	* Calculates the dot product of two vectors with x, y, and z components.
	* @param lhs - Left operand
	* @param rhs - Right operand
	* @return
	*/
	static T Dot3(const Vector4<T>& lhs, const Vector4<T>& rhs);

	/**
	* Calculates the dot product of two vectors with all components.
	* @param lhs - Left operand
	* @param rhs - Right operand
	* @return
	*/
	static T Dot4(const Vector4<T>& lhs, const Vector4<T>& rhs);

	/**
	* Calculates the cross product of two vectors with x, y, z components.
	* W component is always 0.
	* @param lhs - Left operand
	* @param rhs - Right operand
	* @return The cross product vector.
	*/
	static Vector4<T> Cross(const Vector4<T>& lhs, const Vector4<T>& rhs);

public:
	T x; /* X component of the vector */
	T y; /* Y component of the vector */
	T z; /* Z component of the vector */
	T w; /* W component of the vector */
};

using Vector4i = Vector4<int32_t>;	/* Vector type for integers */
using Vector4f = Vector4<float>; /* Vector type for floats */


///////////////////////////////////////////////////////////////////////////
////////////////// Inlined Member Functions ///////////////////////////////
///////////////////////////////////////////////////////////////////////////

template <typename T>
Vector4<T>::Vector4(T X = 0.0f, T Y = 0.0f, T Z = 0.0f, T W = 1.0f)
	: x(X), y(Y), z(Z), w(W){}

template <typename T>
Vector4<T>::Vector4(const Vector3<T>& Vec3)
	: Vector4<T>(Vec3.x, Vec3.y, Vec3.z)
{
}

template <typename T>
template <typename U>
inline Vector4<T>& Vector4<T>::operator*=(const U& scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

template <typename T>
template <typename U>
inline Vector4<T>& Vector4<T>::operator/=(const U& scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}

template <typename T>
inline T& Vector4<T>::operator[](std::size_t idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::out_of_range("Vector4 subscript out of range.");
	};
}

template <typename T>
inline const T& Vector4<T>::operator[](std::size_t idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::out_of_range("Vector4 subscript out of range.");
	};
}

template <typename T>
inline Vector4<T>& Vector4<T>::Normalize3()
{
	float invLength = 1.0f / Length3();
	return *this *= invLength;
}

template <typename T>
inline float Vector4<T>::Length4() const
{
	return sqrtf(x*x + y*y + z*z + w*w);
}

template <typename T>
inline float Vector4<T>::Length3() const
{
	return sqrtf(x*x + y*y + z*z);
}

template <typename T>
inline Vector4<T> Vector4<T>::Reflect3(const Vector4<T>& Normal) const
{
	return 2.0f * Dot3(*this, Normal) * Normal - *this;
}

template <typename T>
void Vector4<T>::Print() const
{
	if (typeid(T) == typeid(float))
		printf("X: %.3f Y: %.3f Z: %.3f W: %.3f", x, y, z, w);
	else if (typeid(T) == typeid(int))
		printf("X: %i Y: %i Z: %i W: %i", x, y, z, w);
}

///////////////////////////////////////////////////////////////////////////
////////////////// Non-Member Functions ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////

template <typename T>
/**
* Calculates the dot product of two vectors with x, y, and z components.
* @param lhs - Left operand
* @param rhs - Right operand
* @return
*/
inline T Vector4<T>::Dot3(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename T>
/**
* Calculates the dot product of two vectors with all components.
* @param lhs - Left operand
* @param rhs - Right operand
* @return
*/
inline T Vector4<T>::Dot4(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return Dot3(lhs, rhs) + lhs.w * rhs.w;
}

template <typename T>
/**
* Calculates the cross product of two vectors with x, y, z components.
* @param lhs - Left operand
* @param rhs - Right operand
* @return The cross product vector.
*/
inline Vector4<T> Vector4<T>::Cross(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return Vector4<T>(	lhs.y * rhs.z - lhs.z * rhs.y,
						lhs.z * rhs.x - lhs.x * rhs.z,
						lhs.x * rhs.y - lhs.y * rhs.x,
						0.0f);
}

template <typename T>
/**
* Adds two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Addition of the two vectors.
*/
inline Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return Vector4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template <typename T>
/**
* Calculates the difference of two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Difference of the two vectors.
*/
inline Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return Vector4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template <typename T>
/**
* Negates the vector components.
* @param lhs - Left operand
* @return The negated vector.
*/
inline Vector4<T> operator-(const Vector4<T>& lhs)
{
	return Vector4<T>(-lhs.x, -lhs.y, -lhs.z, -lhs.w);
}

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param Vec - Left operand (vector)
* @param Scalar - Right operand (scalar)
* @return Memberwise multiplied vector
*/
inline Vector4<T> operator*(const Vector4<T>& Vec, const U& Scalar)
{
	return Vector4<T>(Vec.x * Scalar, Vec.y * Scalar, Vec.z * Scalar, Vec.w * Scalar);
}

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param scalar - Left operand (scalar)
* @param vector - Right operand (vector)
* @return Memberwise multiplied vector
*/
inline Vector4<T> operator*(const U& scalar, const Vector4<T>& vector)
{
	return vector * scalar;
}

template <typename T, typename U>
/**
* Performs vector division.
* @param Vec - Vector to divide.
* @param Scalar - Unit to divide by.
* @return Memberwise divided vector
*/
inline Vector4<T> operator/(const Vector4<T>& Vec, const U& Scalar)
{
	return Vector4<T>(Vec.x / Scalar, Vec.y / Scalar, Vec.z / Scalarz, Vec.w / Scalar);
}