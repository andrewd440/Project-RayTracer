/*
#include "Transform.h"

inline Transform::Transform()
	: mTransformMatrix()
{

}

inline Transform::Transform(Vector3f WorldPosition)
	: mTransformMatrix()
{
	mTransformMatrix.m41 = WorldPosition.x;
	mTransformMatrix.m42 = WorldPosition.y;
	mTransformMatrix.m43 = WorldPosition.z;
}

inline void Transform::rotateX(float Degrees)
{
	float rotationRadians = (Degrees / 180) * 3.14159265;


}

inline void Transform::rotateY(float Degrees)
{
	float rotationRadians = (Degrees / 180) * 3.14159265;


}


inline void Transform::rotateZ(float Degrees)
{
	float rotationRadians = (Degrees / 180) * 3.14159265;


}


inline void Transform::move(const Vector3f& Destination)
{

}

Vector3f Transform::getWorldPosition() const
{

}

inline Transform& Transform::operator*=(const Transform& rhs)
{

}

template <typename T>
inline Vector3<T> operator*(const Transform& lhs, Vector3<T> rhs)
{

}

inline Transform operator*(Transform lhs, const Transform& rhs)
{
	return lhs *= rhs;
}

inline Matrix operator*(const Transform& lhs, Matrix rhs)
{
	rhs.m11 = lhs.getMatrix().m11 * rhs.m11 + lhs.getMatrix().m12 * rhs.m21 + lhs.getMatrix().m13 * rhs.m31;
	rhs.m12 = lhs.getMatrix().m11 * rhs.m12 + lhs.getMatrix().m12 * rhs.m22 + lhs.getMatrix().m13 * rhs.m32;
	rhs.m13 = lhs.getMatrix().m11 * rhs.m13 + lhs.getMatrix().m12 * rhs.m23 + lhs.getMatrix().m13 * rhs.m33;

	rhs.m21 = lhs.getMatrix().m21 * rhs.m11 + lhs.getMatrix().m22 * rhs.m21 + lhs.getMatrix().m23 * rhs.m31;
	rhs.m22 = lhs.getMatrix().m21 * rhs.m12 + lhs.getMatrix().m22 * rhs.m22 + lhs.getMatrix().m23 * rhs.m32;
	rhs.m23 = lhs.getMatrix().m21 * rhs.m13 + lhs.getMatrix().m22 * rhs.m23 + lhs.getMatrix().m23 * rhs.m33;

	rhs.m31 = lhs.getMatrix().m31 * rhs.m11 + lhs.getMatrix().m32 * rhs.m21 + lhs.getMatrix().m33 * rhs.m31;
	rhs.m32 = lhs.getMatrix().m31 * rhs.m12 + lhs.getMatrix().m32 * rhs.m22 + lhs.getMatrix().m33 * rhs.m32;
	rhs.m33 = lhs.getMatrix().m31 * rhs.m13 + lhs.getMatrix().m32 * rhs.m23 + lhs.getMatrix().m33 * rhs.m33;
}

inline Ray operator*(const Transform& lhs, Ray rhs)
{

}
*/