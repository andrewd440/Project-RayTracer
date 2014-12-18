#include "Sphere.h"
#include "Intersection.h"
#include "Camera.h"

FSphere::FSphere(Vector3f Center, float Radius, FMaterial LightingMaterial)
	: IDrawable(LightingMaterial)
	, mRadius(Radius)
{
	SetOrigin(Center);
	ConstructAABB();
}

bool FSphere::IsIntersectingRay(FRay ray, float* tValueOut, FIntersection* intersectionOut)
{ 
	// bring ray into object space
	ray = GetWorldInvTransform().TransformRay(ray);
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
	if (smallestTValue < _EPSILON)
		return false;

	// Only modify t parameter if this intersection's t value is smaller, so we always
	// have the closest intersection
	if (tValueOut && smallestTValue < *tValueOut)
	{
		*tValueOut = smallestTValue;

		// Construct intersection in world space with t solution
		Vector3f intersection = rayOrigin + smallestTValue * rayDirection;
		intersection = GetWorldTransform().TransformPosition(intersection);

		ConstructIntersection(intersection, *intersectionOut);
	}

	return true;
		
}

FMaterial FSphere::GetMaterial(Vector3f SurfacePoint)
{
	return mMaterial;
}

void FSphere::ConstructIntersection(Vector3f IntersectionPoint, FIntersection& IntersectionOut)
{
	IntersectionOut.normal = (IntersectionPoint - GetOrigin()).Normalize();
	IntersectionOut.point = IntersectionPoint;
	IntersectionOut.object = this;
}

float FSphere::GetRadius() const
{
	return mRadius;
}

void FSphere::SetRadius(const float& radius)
{
	mRadius = radius;
}

void FSphere::ConstructAABB(Vector3f Min, Vector3f Max)
{
	SetBoundingBox(AABB(Vector3f(-mRadius, -mRadius, -mRadius), Vector3f(mRadius, mRadius, mRadius)));
}