#pragma once

template <typename T>
class Vector3
{
public:
	Vector3(T x, T y, T z);
	~Vector3();

	Vector3<T>& operator=(const Vector3<T>& rhs);

	Vector3<T>& operator*=(const Vector3<T>& rhs);

	Vector3<T>& operator+=(const Vector3<T>& rhs);

	Vector3<T>& operator-=(const Vector3<T>& rhs);

	void normalize();

	float length();

public:
	T x;
	T y;
	T z;

public:
	typedef Vector3<int> Vectori;
	typedef Vector3<float> Vectorf;
};

template <typename T>
T dotProduct(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T>
Vector3<T> crossProduct(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T>
Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T>
Vector3<T> operator-(const Vector < T>3 & lhs, const Vector3<T>& rhs);