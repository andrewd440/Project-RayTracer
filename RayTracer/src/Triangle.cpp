#include "Triangle.h"
#include "Vector3.inl"

Triangle::Triangle(Vector3f V1, Vector3f V2, Vector3f V3, const Material& LightingMaterial)
	: Shape(LightingMaterial)
	, mVertex1(V1)
	, mVertex2(V2)
	, mVertex3(V3)
{
	Vector3f e1 = V3 - V2;
	Vector3f e2 = V1 - V3;
	mNormal = crossProduct(e1, e2);
	mNormal.normalize();
}

bool Triangle::isIntersectingRay(Ray Ray, float& tValueOut, Intersection& IntersectionOut)
{
	return false;
}

bool Triangle::isIntersectingRay(Ray Ray)
{
	return false;
}
