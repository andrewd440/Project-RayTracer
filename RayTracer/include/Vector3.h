#pragma once

#include <cstdint>
#include <cmath>
#include <iostream>
#include <stdexcept>

template <typename T>
/**
* Class of manipulating a vector with 3 components.
*/
class TVector3
{
public:
	/**
	* Constructs vector with x, y, z coordinates.
	* @param X - position of X coordinate
	* @param Y - position of Y coordinate
	* @param Z - position of Z coordinate
	*/
	TVector3(T X = 0, T Y = 0, T Z = 0);

	template <typename U>
	/**
	* Performs vector-scalar multiplication.
	* @param scalar - Scalar to multiply by.
	* @return Reference to this vector.
	*/
	TVector3<T>& operator*=(const U& Scalar);

	/**
	* Performs vector addition.
	* @param rhs - TVector3 to add.
	* @return Reference to this vector.
	*/
	TVector3<T>& operator+=(const TVector3<T>& rhs);

	/**
	* Performs vector subtraction.
	* @param rhs - TVector3 to subtract
	* @return Reference to this vector
	*/
	TVector3<T>& operator-=(const TVector3<T>& rhs);

	template <typename U>
	/**
	* Performs vector division.
	* @param rhs - Unit to divide by.
	* @return Reference to this vector
	*/
	TVector3<T>& operator/=(const U& Scalar);

	/**
	* Checks vector equality.
	*/
	bool operator==(const TVector3<T>& Rhs) const;

	/**
	* Checks vector inequality.
	*/
	bool operator!=(const TVector3<T>& Rhs) const;

	/**
	* Overload of subscript operators for X = 0, Y = 1, Z = 2.
	*/
	T& operator[](std::size_t idx);

	/**
	* Overload of subscript operators for X = 0, Y = 1, Z = 2.
	*/
	const T& operator[](std::size_t idx) const;

	/**
	* Normalizes the vector.
	*/
	TVector3<T>& Normalize();

	/**
	* Calculates the length/magnatude of the vector.
	* @return The length/magnatude
	*/
	float Length() const;

	/**
	* Reflects the vector across a normal.
	* @return Reflected vector
	*/
	TVector3<T> Reflect(const TVector3<T>& Normal) const;

	/* Prints the string representation of the vector */
	void Print();

	/**
	* Calculates the dot product of two vectors.
	* @param lhs - Left operand
	* @param rhs - Right operand
	* @return
	*/
	static T Dot(const TVector3<T>& lhs, const TVector3<T>& rhs);

	/**
	* Calculates the cross product of two vectors.
	* @param lhs - Left operand
	* @param rhs - Right operand
	* @return The cross product vector.
	*/
	static TVector3<T> Cross(const TVector3<T>& lhs, const TVector3<T>& rhs);

public:
	T x; /* X coordinate of the vector */
	T y; /* Y coordinate of the vector */
	T z; /* Z coordinate of the vector */
};

using Vector3i = TVector3<int32_t>;	/* Vector type for integers */
using Vector3f = TVector3<float>; /* Vector type for floats */


/////////////////////////////////////////////////////
//////////// Inlined Member Functions ///////////////
/////////////////////////////////////////////////////


template <typename T>
inline TVector3<T>::TVector3(T X, T Y, T Z)
	: x(X), y(Y), z(Z) {}


template <typename T>
template <typename U>
inline TVector3<T>& TVector3<T>::operator*=(const U& Scalar)
{
	x *= Scalar;
	y *= Scalar;
	z *= Scalar;

	return *this;
}

template <typename T>
template <typename U>
TVector3<T>& TVector3<T>::operator/=(const U& Scalar)
{
	x /= Scalar;
	y /= Scalar;
	z /= Scalar;

	return *this;
}

template <typename T>
inline TVector3<T>& TVector3<T>::operator+=(const TVector3<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

template <typename T>
inline TVector3<T>& TVector3<T>::operator-=(const TVector3<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

template <typename T>
inline bool TVector3<T>::operator==(const TVector3<T>& Rhs) const
{
	for (size_t i = 0; i < 3; i++)
		if ((*this)[i] != Rhs[i])
			return false;

	return true;
}

template <typename T>
inline bool TVector3<T>::operator!=(const TVector3<T>& Rhs) const
{
	for (size_t i = 0; i < 3; i++)
		if ((*this)[i] != Rhs[i])
			return true;

	return false;
}

template <typename T>
T& TVector3<T>::operator[](std::size_t idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::out_of_range("TVector3 subscript out of range.");
	};
}

template <typename T>
const T& TVector3<T>::operator[](std::size_t idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::out_of_range("TVector3 subscript out of range.");
	};
}

template <typename T>
inline TVector3<T>& TVector3<T>::Normalize()
{
	return *this /= Length();
}


template <typename T>
inline float TVector3<T>::Length() const
{
	return std::sqrt(x*x + y*y + z*z);
}

template <typename T>
TVector3<T> TVector3<T>::Reflect(const TVector3<T>& Normal) const
{
	return 2 * Dot(*this, Normal) * Normal - *this;
}

template <typename T>
void TVector3<T>::Print()
{
	if (typeid(T) == typeid(float))
		printf("X: %.3f Y: %.3f Z: %.3f", x, y, z);
	else if (typeid(T) == typeid(int))
		printf("X: %i Y: %i Z: %i", x, y, z);
}

template <typename T>
inline T TVector3<T>::Dot(const TVector3<T>& lhs, const TVector3<T>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename T>
inline TVector3<T> TVector3<T>::Cross(const TVector3<T>& lhs, const TVector3<T>& rhs)
{
	return TVector3<T>(	(lhs.y * rhs.z) - (lhs.z * rhs.y),
						(lhs.z * rhs.x) - (lhs.x * rhs.z),
						(lhs.x * rhs.y) - (lhs.y * rhs.x));
}

/////////////////////////////////////////////////////////////////////
//////////////// Non-member Functions ///////////////////////////////
/////////////////////////////////////////////////////////////////////

template <typename T>
/**
* Adds two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Addition of the two vectors.
*/
inline TVector3<T> operator+(const TVector3<T>& lhs, const TVector3<T>& rhs)
{
	return TVector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <typename T>
/**
* Calculates the difference of two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Difference of the two vectors.
*/
inline TVector3<T> operator-(const TVector3<T>& lhs, const TVector3<T>& rhs)
{
	return TVector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template <typename T>
/**
* Negates the vector components.
* @param lhs - Left operand
* @return The negated vector.
*/
inline TVector3<T> operator-(const TVector3<T>& lhs)
{
	return TVector3<T>(-lhs.x, -lhs.y, -lhs.z);
}

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param Vec - Left operand (vector)
* @param Scalar - Right operand (scalar)
* @return Memberwise multiplied vector
*/
inline TVector3<T> operator*(const TVector3<T>& Vec, const U& Scalar)
{
	return TVector3<T>(Vec.x * Scalar, Vec.y * Scalar, Vec.z * Scalar);
}

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param scalar - Left operand (scalar)
* @param vector - Right operand (vector)
* @return Memberwise multiplied vector
*/
inline TVector3<T> operator*(const U& Scalar, const TVector3<T>& Vector)
{
	return Vector * Scalar;
}

template <typename T, typename U>
/**
* Performs vector division.
* @param Vec - Vector to divide.
* @param Scalar - Unit to divide by.
* @return Memberwise divided vector
*/
inline TVector3<T> operator/(const TVector3<T>& Vec, const U& Scalar)
{
	return TVector3<T>(Vec.x / Scalar, Vec.y / Scalar, Vec.z / Scalar);
}