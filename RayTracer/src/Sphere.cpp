#include "Sphere.h"
#include "Intersection.h"
#include "LocalGeometry.h"
#include "Vector3.inl"

Sphere::Sphere(Vector3f center, float radius)
	: mCenter(center)
	, mRadius(radius)
{

}

bool Sphere::isIntersectingRay(Ray ray, float tMax, Intersection& intersectionOut)
{
	const Vector3f rayDirection = ray.direction;
	const Vector3f rayOrigin = ray.origin;

	// Coefficients for quandratic equation
	const float a = dotProduct(rayDirection, rayDirection);
	const float b = dotProduct(2 * (rayOrigin - mCenter), rayDirection);
	const float c = dotProduct((rayOrigin - mCenter), (rayOrigin - mCenter)) - mRadius * mRadius;

	// Find discriminate
	const float discriminate = (b * b) - (4 * a * c);

	// If discriminate is less than zero, there is no real roots (no intersection)
	if (discriminate < 0)
		return false;

	// Compute t0 and t1
	float discriminateSqrt = std::sqrtf(discriminate);
	float q;
	if (b < 0)
		q = (-b - discriminateSqrt) / 2.f;
	else
		q = (-b + discriminateSqrt) / 2.f;

	float t0 = q / a;
	float t1 = c / q;

	// Make sure larger t is in t1
	if (t0 > t1)
	{
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	// If the larger t is negative, the sphere is behind the ray
	if (t1 < 0)
		return false;
	
	// If the smaller t is negative, t1 is the solution
	if (t0 < 0)
	{
		// Construct the intersection using ray equation and solution t1
		constructIntersection(rayOrigin + rayDirection*t1, intersectionOut);
		return true;
	}
	// t0 is the solution
	else
	{
		// Construct the intersection using ray equation and solution t0
		constructIntersection(rayOrigin + rayDirection*t0, intersectionOut);
		return true;
	}
		
}

Vector3f Sphere::getCenter() const
{
	return mCenter;
}

void Sphere::setCenter(const Vector3f& center)
{
	mCenter = center;
}

float Sphere::getRadius() const
{
	return mRadius;
}

void Sphere::setRadius(const float& radius)
{
	mRadius = radius;
}

void Sphere::constructIntersection(Vector3f point, Intersection& intersectionOut)
{
	LocalGeometry& geometry = intersectionOut.localGeometry;
	geometry.point = point;

	Vector3f normal = point - mCenter;
	normal.normalize();
	geometry.surfaceNormal = normal;

	
	intersectionOut.object = this;
}