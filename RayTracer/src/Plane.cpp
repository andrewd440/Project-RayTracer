#include "Plane.h"
#include "Intersection.h"
#include "Vector3.inl"


Plane::Plane(const Material& LightingMaterial, Vector3f PlaneNormal, Vector3f PointOnPlane)
	: Shape(LightingMaterial)
	, mNormal(PlaneNormal)
	, mDistanceFromOrigin()
{
	mNormal.normalize();
	mDistanceFromOrigin = dotProduct(-PlaneNormal, PointOnPlane);
}

bool Plane::isIntersectingRay(Ray Ray, float* tValueOut, Intersection* IntersectionOut)
{
	if (dotProduct(Ray.direction, mNormal) == 0)
		return false;

	// Plane/Ray intersection from Mathmatics for 3D Game Programming and Computer Graphics
	float tValue = -(dotProduct(mNormal, Ray.origin) + mDistanceFromOrigin) / (dotProduct(mNormal, Ray.direction));

	if (tValue <= .01f)
		return false;

	if (tValueOut && tValue < *tValueOut)
	{
		*tValueOut = tValue;
		constructIntersection(Ray.origin + Ray.direction * tValue, *IntersectionOut);
	}

	return true;
}

void Plane::constructIntersection(Vector3f IntersectionPoint, Intersection& IntersectionOut)
{
	IntersectionOut.object = this;
	IntersectionOut.localGeometry.surfaceNormal = mNormal;
	IntersectionOut.localGeometry.point = IntersectionPoint;
}