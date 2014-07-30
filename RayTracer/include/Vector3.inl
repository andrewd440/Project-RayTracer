#include <cmath>
#include <iostream>

template <typename T>
inline Vector3<T>::Vector3()
	: x(0), y(0), z(0) {}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector3<T>::Vector3(T X, T Y, T Z)
	: x(X), y(Y), z(Z) {}

////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename U>
inline Vector3<T>& Vector3<T>::operator*=(const U& scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename U>
Vector3<T>& Vector3<T>::operator/=(const U& scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline void Vector3<T>::normalize()
{
	*this /= length();
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline float Vector3<T>::length()
{
	return std::sqrt(x*x + y*y + z*z);
}

////////////////////////////////////////////////////////////////////////

template <typename T>
void Vector3<T>::toString()
{
	std::cout << "Vector: [" << x << " " << y << " " << z << "]\n";
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline T dotProduct(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector3<T> crossProduct(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	Vector3<T> cross;
	cross.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
	cross.y = (lhs.z * rhs.x) - (lhs.x * rhs.z);
	cross.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);

	return cross;
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

////////////////////////////////////////////////////////////////////////

template <typename T>
inline Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

////////////////////////////////////////////////////////////////////////

template <typename T>
Vector3<T> operator-(const Vector3<T>& lhs)
{
	return Vector3<T>(-lhs.x, -lhs.y, -lhs.z);
}

////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
inline Vector3<T> operator*(const Vector3<T>& vector, const U& scalar)
{
	return Vector3<T>(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
inline Vector3<T> operator*(const U& scalar, const Vector3<T>& vector)
{
	return Vector3<T>(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
Vector3<T> operator/(const Vector3<T>& vector, const U& scalar)
{
	return Vector3<T>(vector.x / scalar, vector.y / scalar, vector.z / scalar);
}