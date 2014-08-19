#pragma once

#include "Vector3.h"
/*

#include "Ray.h"
#include "LocalGeometry.h"
#include "Matrix.h"

class Transform
{
public:
	Transform();

	Transform(Vector3f WorldPosition);

	void rotateX(float Degrees);
	void rotateY(float Degrees);
	void rotateZ(float Degrees);

	void move(const Vector3f& Destination);
	Vector3f getWorldPosition() const;

	Transform& operator*=(const Transform& rhs);

	Matrix getMatrix() const;


private:
	Matrix mTransformMatrix;
};

template <typename T>
Vector3<T> operator*(const Transform& lhs, Vector3<T> rhs);

Matrix operator*(const Transform& lhs, Matrix rhs);

Transform operator*(Transform lhs, const Transform& rhs);

Ray operator*(const Transform& lhs, Ray rhs);

*/