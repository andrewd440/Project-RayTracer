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

public:
	T x; /* X coordinate of the vector */
	T y; /* Y coordinate of the vector */
};

typedef Vector2<int32_t> Vector2i;	/* Vector type for integers */
typedef Vector2<float> Vector2f; /* Vector type for floats */