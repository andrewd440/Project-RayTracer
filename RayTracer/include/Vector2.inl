#include <cmath>
#include <iostream>
#include <stdexcept>

template <typename T>
template <typename U>
inline Vector2<T>& Vector2<T>::operator*=(const U& scalar)
{
	x *= scalar;
	y *= scalar;

	return *this;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename U>
Vector2<T>& Vector2<T>::operator/=(const U& scalar)
{
	x /= scalar;
	y /= scalar;

	return *this;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
T& Vector2<T>::operator[](std::size_t idx)
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

////////////////////////////////////////////////////////////////////////

template <typename T>
const T& Vector2<T>::operator[](std::size_t idx) const
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

////////////////////////////////////////////////////////////////////////

template <typename T>
inline void Vector2<T>::normalize()
{
	*this /= length();
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline float Vector2<T>::length()
{
	return std::sqrt(x*x + y*y);
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline T dotProduct(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
	return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
	return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& lhs)
{
	return Vector2<T>(-lhs.x, -lhs.y);
}

////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
inline Vector2<T> operator*(const Vector2<T>& vector, const U& scalar)
{
	return Vector2<T>(vector.x * scalar, vector.y * scalar);
}

////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
inline Vector2<T> operator*(const U& scalar, const Vector2<T>& vector)
{
	return Vector2<T>(vector.x * scalar, vector.y * scalar);
}

////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
inline Vector2<T> operator/(const Vector2<T>& vector, const U& scalar)
{
	return Vector2<T>(vector.x / scalar, vector.y / scalar);
}