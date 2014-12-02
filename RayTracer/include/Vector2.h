#pragma once

#include <cstdint>

template <typename T>
/**
* Class of manipulating a 2D vector.
*/
class Vector2
{
public:
	/**
	* Default constructor. x=y=0
	*/
	Vector2()
		: x(0), y(0) {}

	/**
	* Constructs vector with x, y coordinates.
	* @param X - position of X coordinate
	* @param Y - position of Y coordinate
	*/
	Vector2(T X, T Y)
		: x(X), y(Y) {}

	template <typename U>
	/**
	* Performs vector-scalar multiplication.
	* @param scalar - Scalar to multiply by.
	* @return Reference to this vector.
	*/
	Vector2<T>& operator*=(const U& scalar);

	/**
	* Performs vector addition.
	* @param rhs - Vector2 to add.
	* @return Reference to this vector.
	*/
	Vector2<T>& operator+=(const Vector2<T>& rhs);

	/**
	* Performs vector subtraction.
	* @param rhs - Vector2 to subtract
	* @return Reference to this vector
	*/
	Vector2<T>& operator-=(const Vector2<T>& rhs);

	template <typename U>
	/**
	* Performs vector division.
	* @param rhs - Unit to divide by.
	* @return Reference to this vector
	*/
	Vector2<T>& operator/=(const U& scalar);

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
	void normalize();

	/**
	* Calculates the length/magnatude of the vector.
	* @return The length/magnatude
	*/
	float length();

public:
	T x; /* X coordinate of the vector */
	T y; /* Y coordinate of the vector */
};

typedef Vector2<int32_t> Vector2i;	/* Vector type for integers */
typedef Vector2<float> Vector2f; /* Vector type for floats */

template <typename T>
/**
* Calculates the dot product of two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return
*/
T dotProduct(const Vector2<T>& lhs, const Vector2<T>& rhs);

template <typename T>
/**
* Adds two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Addition of the two vectors.
*/
Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs);

template <typename T>
/**
* Calculates the difference of two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Difference of the two vectors.
*/
Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs);

template <typename T>
/**
* Negates the vector components.
* @param lhs - Left operand
* @return The negated vector.
*/
Vector2<T> operator-(const Vector2<T>& lhs);

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param vector - Left operand (vector)
* @param scalar - Right operand (scalar)
* @return Memberwise multiplied vector
*/
Vector2<T> operator*(const Vector2<T>& vector, const U& scalar);

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param scalar - Left operand (scalar)
* @param vector - Right operand (vector)
* @return Memberwise multiplied vector
*/
Vector2<T> operator*(const U& scalar, const Vector2<T>& vector);

template <typename T, typename U>
/**
* Performs vector division.
* @param rhs - Unit to divide by.
* @return Memberwise divided vector
*/
Vector2<T> operator/(const Vector2<T>& vector, const U& scalar);