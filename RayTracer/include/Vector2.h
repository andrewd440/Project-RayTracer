#pragma once

#include <cstdint>
#include <cmath>
#include <iostream>
#include <stdexcept>

template <typename T>
/**
* Class of manipulating a 2D vector.
*/
class TVector2
{
public:
	/**
	* Constructs vector with 2 components.
	* @param X - value of X component
	* @param Y - value of Y component
	*/
	TVector2(T X = 0, T Y = 0);

	template <typename U>
	/**
	* Performs vector-scalar multiplication.
	* @param Scalar - Scalar to multiply by.
	* @return Reference to this vector.
	*/
	TVector2<T>& operator*=(const U& Scalar);

	/**
	* Performs vector addition.
	* @param rhs - Vector2 to add.
	* @return Reference to this vector.
	*/
	TVector2<T>& operator+=(const TVector2<T>& rhs);

	/**
	* Performs vector subtraction.
	* @param rhs - Vector2 to subtract
	* @return Reference to this vector
	*/
	TVector2<T>& operator-=(const TVector2<T>& rhs);

	template <typename U>
	/**
	* Performs vector division.
	* @param Scalar - Unit to divide by.
	* @return Reference to this vector
	*/
	TVector2<T>& operator/=(const U& Scalar);

	/**
	* Checks vector equality.
	*/
	bool operator==(const TVector2<T>& Rhs) const;

	/**
	* Checks vector inequality.
	*/
	bool operator!=(const TVector2<T>& Rhs) const;

	/**
	* Overload of subscript operators for X = 0, Y = 1.
	*/
	T& operator[](std::size_t idx);

	/**
	* Overload of subscript operators for X = 0, Y = 1.
	*/
	const T& operator[](std::size_t idx) const;

	/**
	* Normalizes the vector.
	*/
	void Normalize();

	/**
	* Calculates the length/magnatude of the vector.
	* @return The length/magnatude
	*/
	float Length();

	/**
	* Reflects the vector across a normal.
	* @return Reflected vector
	*/
	TVector2<T> Reflect(const TVector2<T>& Normal) const;

	/* Prints the string representation of the vector */
	void Print();

	/**
	* Calculates the dot product of two vectors.
	* @param lhs - Left operand
	* @param rhs - Right operand
	* @return The dot product
	*/
	static T Dot(const TVector2<T>& lhs, const TVector2<T>& rhs);

public:
	T x; /* X coordinate of the vector */
	T y; /* Y coordinate of the vector */
};

using Vector2i = TVector2<int32_t>;	/* Vector type for integers */
using Vector2f = TVector2<float>; /* Vector type for floats */

/////////////////////////////////////////////////////
//////////// Inlined Member Functions ///////////////
/////////////////////////////////////////////////////

template <typename T>
TVector2<T>::TVector2(T X = 0, T Y = 0)
	: x(X), y(Y) {}

template <typename T>
template <typename U>
inline TVector2<T>& TVector2<T>::operator*=(const U& Scalar)
{
	x *= Scalar;
	y *= Scalar;

	return *this;
}


template <typename T>
template <typename U>
TVector2<T>& TVector2<T>::operator/=(const U& Scalar)
{
	x /= Scalar;
	y /= Scalar;

	return *this;
}


template <typename T>
inline TVector2<T>& TVector2<T>::operator+=(const TVector2<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}


template <typename T>
inline TVector2<T>& TVector2<T>::operator-=(const TVector2<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

template <typename T>
inline bool TVector2<T>::operator==(const TVector2<T>& Rhs) const
{
	for (size_t i = 0; i < 2; i++)
		if (*this[i] != Rhs[i])
			return false;

	return true;
}

template <typename T>
inline bool TVector2<T>::operator!=(const TVector2<T>& Rhs) const
{
	for (size_t i = 0; i < 4; i++)
		if ((*this)[i] != Rhs[i])
			return true;

	return false;
}

template <typename T>
T& TVector2<T>::operator[](std::size_t idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Vector2 subscript out of range.");
	};
}


template <typename T>
const T& TVector2<T>::operator[](std::size_t idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Vector2 subscript out of range.");
	};
}


template <typename T>
inline void TVector2<T>::Normalize()
{
	*this /= Length();
}


template <typename T>
inline float TVector2<T>::Length()
{
	return std::sqrt(x*x + y*y);
}

template <typename T>
void TVector2<T>::Print()
{
	if (typeid(T) == typeid(float))
		printf("X: %.3f Y: %.3f", x, y);
	else if (typeid(T) == typeid(int))
		printf("X: %i Y: %i", x, y);
}

template <typename T>
inline T Dot(const TVector2<T>& lhs, const TVector2<T>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
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
inline TVector2<T> operator+(const TVector2<T>& lhs, const TVector2<T>& rhs)
{
	return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <typename T>
/**
* Calculates the difference of two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Difference of the two vectors.
*/
inline TVector2<T> operator-(const TVector2<T>& lhs, const TVector2<T>& rhs)
{
	return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <typename T>
/**
* Negates the vector components.
* @param lhs - Left operand
* @return The negated vector.
*/
inline TVector2<T> operator-(const TVector2<T>& lhs)
{
	return Vector2<T>(-lhs.x, -lhs.y);
}

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param Vec - Left operand (vector)
* @param Scalar - Right operand (scalar)
* @return Memberwise multiplied vector
*/
inline TVector2<T> operator*(const TVector2<T>& Vec, const U& Scalar)
{
	return Vector2<T>(Vec.x * Scalar, Vec.y * Scalar);
}

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param Scalar - Left operand (scalar)
* @param vector - Right operand (vector)
* @return Memberwise multiplied vector
*/
inline TVector2<T> operator*(const U& Scalar, const TVector2<T>& Vec)
{
	return Vector2<T>(Vec.x * Scalar, Vec.y * Scalar);
}

template <typename T, typename U>
/**
* Performs vector division.
* @param Vec - Vector to divide.
* @param Scalar - Unit to divide by.
* @return Memberwise divided vector
*/
inline TVector2<T> operator/(const TVector2<T>& Vec, const U& Scalar)
{
	return Vector2<T>(Vec.x / Scalar, Vec.y / Scalar);
}

template <typename T, typename U>
/**
* Performs vector division.
* @param Scalar - Unit to divide.
* @param Vec - Vector to divide by.
* @return Memberwise divided vector
*/
inline TVector2<T> operator/(const U& Scalar, const TVector2<T>& Vec)
{
	return Vector2<T>(Scalar / Vec.x, Scalar / Vec.y);
}