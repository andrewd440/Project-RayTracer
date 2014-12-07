#include "Plane.h"
#include "Intersection.h"
#include "Camera.h"

Plane::Plane(const Material& LightingMaterial, Vector3f PlaneNormal, Vector3f PointOnPlane)
	: Primitive(LightingMaterial)
	, mNormal(PlaneNormal)
	, mDistanceFromOrigin()
{
	// compute normal in view space since ray is coming from there
	const Matrix4& viewTransform = Camera::ViewTransform;
	mNormal = viewTransform.TransformDirection(mNormal).Normalize();
	PointOnPlane = viewTransform.TransformPosition(PointOnPlane);

	mDistanceFromOrigin = Vector3f::Dot(-PlaneNormal, PointOnPlane);
	constructAABB();
}

bool Plane::isIntersectingRay(Ray Ray, float* tValueOut, Intersection* IntersectionOut)
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
		constructIntersection(Ray.origin + Ray.direction * tValue, *IntersectionOut);
	}

	return true;
}

void Plane::constructIntersection(Vector3f IntersectionPoint, Intersection& IntersectionOut)
{
	IntersectionOut.object = this;
	IntersectionOut.normal = mNormal;
	IntersectionOut.point = IntersectionPoint;
}

void Plane::constructAABB()
{
	AABB boundingBox;

		boundingBox.max.x = std::numeric_limits<float>::max();
		boundingBox.min.x = -std::numeric_limits<float>::max();

		boundingBox.max.y = 0;
		boundingBox.min.y = 0;

		boundingBox.max.z = std::numeric_limits<float>::max();
		boundingBox.min.z = -std::numeric_limits<float>::max();

	setBoundingBox(boundingBox);
}