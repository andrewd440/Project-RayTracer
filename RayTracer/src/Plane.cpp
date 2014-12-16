#include "Plane.h"
#include "Intersection.h"
#include "Camera.h"

FPlane::FPlane(const FMaterial& LightingMaterial, Vector3f PlaneNormal, Vector3f PointOnPlane)
	: IDrawable(LightingMaterial)
	, mNormal(PlaneNormal)
	, mDistanceFromOrigin()
{
	mNormal.Normalize();

	mDistanceFromOrigin = Vector3f::Dot(-PlaneNormal, PointOnPlane);
	ConstructAABB();
}

bool FPlane::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	if (Vector3f::Dot(Ray.direction, mNormal) == 0)
		return false;

	// Plane/Ray intersection from Mathmatics for 3D Game Programming and Computer Graphics
	float tValue = -(Vector3f::Dot(mNormal, Ray.origin) + mDistanceFromOrigin) / (Vector3f::Dot(mNormal, Ray.direction));

	if (tValue <= .01f)
		return false;

	if (tValueOut && tValue < *tValueOut)
	{
		*tValueOut = tValue;
		ConstructIntersection(Ray.origin + Ray.direction * tValue, *IntersectionOut);
	}

	return true;
}

void FPlane::ConstructIntersection(Vector3f IntersectionPoint, FIntersection& IntersectionOut)
{
	IntersectionOut.object = this;
	IntersectionOut.normal = mNormal;
	IntersectionOut.point = IntersectionPoint;
}

void FPlane::ConstructAABB(Vector3f Min, Vector3f Max)
{
	Max.x = std::numeric_limits<float>::max();
	Min.x = -std::numeric_limits<float>::max();

	Max.y = 0;
	Min.y = 0;

	Max.z = std::numeric_limits<float>::max();
	Min.z = -std::numeric_limits<float>::max();

	SetBoundingBox(AABB(Min, Max));
}