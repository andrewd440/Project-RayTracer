// RayTracer.cpp : Defines the entry point for the console application.
//

#include "Vector3.h"
#include "Vector3.inl"
#include "Ray.h"
#include "Sphere.h"
#include "Intersection.h"
#include <iostream>

int main()
{
	Vector3f normal(0, 0, 1);
	normal.normalize();
	Ray ray(Vector3f(0, 0, 5), normal);
	Sphere circle(Vector3f(0, 0, 4), 2);
	Intersection intersection;
	float t = 0;
	circle.isIntersectingRay(ray, t, intersection);

	intersection.localGeometry.point.toString();
	std::cout << "t solution: " << t << std::endl;
	intersection.localGeometry.surfaceNormal.toString();
	return 0;
}