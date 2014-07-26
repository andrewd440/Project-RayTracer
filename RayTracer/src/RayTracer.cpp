#include "RayTracer.h"
#include "Intersection.h"
#include "Vector3.inl"

RayTracer::RayTracer()
	: sphere(Vector3f(0,0,10), 5)
{
}

bool RayTracer::trace(const Ray& ray)
{
	float t;
	Intersection intersection;
	return sphere.isIntersectingRay(ray, t, intersection);
}