#include "Sphere.h"
#include "Intersection.h"
#include "Camera.h"

FSphere::FSphere(Vector3f Center, float Radius, FMaterial LightingMaterial)
	: IPrimitive(LightingMaterial)
	, mRadius(Radius)
{
	mTransform.SetOrigin(Center);
	ConstructAABB();
}

bool FSphere::IsIntersectingRay(FRay ray, float* tValueOut, FIntersection* intersectionOut)
{ 
	// bring ray into object space
	ray = mTransform.GetInverseAffine().TransformRay(ray);
	const Vector3f rayDirection = ray.direction;
	const Vector3f rayOrigin = ray.origin;

	// Coefficients for quandratic equation
	const float b = Vector3f::Dot(rayOrigin, rayDirection);
	const float c = Vector3f::Dot(rayOrigin, rayOrigin) - mRadius * mRadius;

	// If the ray origin is outside the sphere and direction is pointing away from sphere
	if (c > 0 && b > 0) 
		return false;

	const float discriminate = (b * b) - c;

	// If discriminate is less than zero, there is no real roots (no intersection)
	if (discriminate < 0)
		return false;

	float smallestTValue = -b - sqrtf(discriminate);

	// If t is negative, the ray started inside the sphere, reject the collision for now
	// Avoid really small values of t to prevent shadow errors
	if (smallestTValue < .01f)
		return false;

	// Only modify t parameter if this intersection's t value is smaller, so we always
	// have the closest intersection
	if (tValueOut && smallestTValue < *tValueOut)
	{
		*tValueOut = smallestTValue;

		// Construct intersection in world space with t solution
		Vector3f intersection = rayOrigin + smallestTValue * rayDirection;
		intersection = mTransform.TransformPosition(intersection);

		ConstructIntersection(intersection, *intersectionOut);
	}

	return true;
		
}

void FSphere::ConstructIntersection(Vector3f IntersectionPoint, FIntersection& IntersectionOut)
{
	IntersectionOut.normal = (IntersectionPoint - GetCenter()).Normalize();
	IntersectionOut.point = IntersectionPoint;
	IntersectionOut.object = this;
}

Vector3f FSphere::GetCenter() const
{
	return mTransform.GetOrigin();
}

void FSphere::SetCenter(const Vector3f& CenterPoint)
{
	mTransform.SetOrigin(CenterPoint);
}

float FSphere::GetRadius() const
{
	return mRadius;
}

void FSphere::SetRadius(const float& radius)
{
	mRadius = radius;
}

void FSphere::ConstructAABB()
{
	AABB boundingBox;
	boundingBox.max = Vector3f(mRadius, mRadius, -mRadius);
	boundingBox.min = Vector3f(-mRadius, -mRadius, mRadius);

	setBoundingBox(boundingBox);
}