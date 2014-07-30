#pragma once

template <typename T>
/**
* Class of manipulating a 3D vector.
*/
class Vector3
{
public:
	/**
	* Default constructor. x=y=z=0
	*/
	Vector3();

	/**
	* Constructs vector with x, y, z coordinates.
	* @param X - position of X coordinate
	* @param Y - position of Y coordinate
	* @param Z - position of Z coordinate
	*/
	Vector3(T X, T Y, T Z);

	template <typename U>
	/**
	* Performs vector-scalar multiplication.
	* @param scalar - Scalar to multiply by.
	* @return Reference to this vector.
	*/
	Vector3<T>& operator*=(const U& scalar);

	/**
	* Performs vector addition.
	* @param rhs - Vector3 to add.
	* @return Reference to this vector.
	*/
	Vector3<T>& operator+=(const Vector3<T>& rhs);

	/**
	* Performs vector subtraction.
	* @param rhs - Vector3 to subtract
	* @return Reference to this vector
	*/
	Vector3<T>& operator-=(const Vector3<T>& rhs);

	template <typename U>
	/**
	* Performs vector division.
	* @param rhs - Unit to divide by.
	* @return Reference to this vector
	*/
	Vector3<T>& operator/=(const U& scalar);

	/**
	* Normalizes the vector.
	*/
	void normalize();

	/**
	* Calculates the length/magnatude of the vector.
	* @return The length/magnatude
	*/
	float length();

	/* Prints the coordinates of the vector */
	void toString();

public:
	T x; /* X coordinate of the vector */
	T y; /* Y coordinate of the vector */
	T z; /* Z coordinate of the vector */
};

typedef Vector3<int> Vector3i;	/* Vector type for integers */
typedef Vector3<float> Vector3f; /* Vector type for floats */

template <typename T>
/**
* Calculates the dot product of two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return
*/
T dotProduct(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T>
/**
* Calculates the cross product of two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return The cross product vector.
*/
Vector3<T> crossProduct(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T>
/**
* Adds two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Addition of the two vectors.
*/
Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T>
/**
* Calculates the difference of two vectors.
* @param lhs - Left operand
* @param rhs - Right operand
* @return Difference of the two vectors.
*/
Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T>
/**
* Negates the vector components.
* @param lhs - Left operand
* @return The negated vector.
*/
Vector3<T> operator-(const Vector3<T>& lhs);

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param vector - Left operand (vector)
* @param scalar - Right operand (scalar)
* @return Memberwise multiplied vector
*/
Vector3<T> operator*(const Vector3<T>& vector, const U& scalar);

template <typename T, typename U>
/**
* Performs vector-scalar multiplication of two vectors.
* @param scalar - Left operand (scalar)
* @param vector - Right operand (vector)
* @return Memberwise multiplied vector
*/
Vector3<T> operator*(const U& scalar, const Vector3<T>& vector);

//template <typename T>
//Vector3<T> operator*(const Vector <T>3 & lhs, const Transform& rhs);

template <typename T, typename U>
/**
* Performs vector division.
* @param rhs - Unit to divide by.
* @return Memberwise divided vector
*/
Vector3<T> operator/(const Vector3<T>& vector, const U& scalar);