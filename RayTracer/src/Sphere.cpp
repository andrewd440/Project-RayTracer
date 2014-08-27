#include "Sphere.h"
#include "Intersection.h"
#include "LocalGeometry.h"
#include "Vector3.inl"

Sphere::Sphere(Vector3f center, float radius, Material LightingMaterial)
	: Shape(LightingMaterial)
	, mCenter(center)
	, mRadius(radius)
{

}

bool Sphere::isIntersectingRay(Ray ray, float* tValueOut, Intersection* intersectionOut)
{
	const Vector3f rayDirection = ray.direction;
	const Vector3f rayOrigin = ray.origin;
	const Vector3f m = rayOrigin - mCenter;

	// Coefficients for quandratic equation
	const float b = dotProduct(m, rayDirection);
	const float c = dotProduct(m, m) - mRadius * mRadius;

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

		// Construct intersection with t solution
		constructIntersection(rayOrigin + smallestTValue * rayDirection, *intersectionOut);
	}

	return true;
		
}

Vector3f Sphere::getCenter() const
{
	return mCenter;
}

void Sphere::setCenter(const Vector3f& CenterPoint)
{
	mCenter = CenterPoint;
}

float Sphere::getRadius() const
{
	return mRadius;
}

void Sphere::setRadius(const float& radius)
{
	mRadius = radius;
}

void Sphere::constructIntersection(Vector3f IntersectionPoint, Intersection& IntersectionOut)
{
	LocalGeometry& geometry = IntersectionOut.localGeometry;

	Vector3f normal = IntersectionPoint - mCenter;
	normal.normalize();
	geometry.surfaceNormal = normal;

	geometry.point = IntersectionPoint;
	
	IntersectionOut.object = this;
}