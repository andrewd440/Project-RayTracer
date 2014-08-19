#pragma once
#include "Shape.h"
#include "Vector3.h"

class Triangle : public Shape
{
public:
	Triangle(Vector3f V1, Vector3f V2, Vector3f V3, const Material& LightingMaterial);
	
	bool isIntersectingRay(Ray Ray, float& tValueOut, Intersection& IntersectionOut);

	bool isIntersectingRay(Ray Ray);

private:
	Vector3f mVertex1;
	Vector3f mVertex2;
	Vector3f mVertex3;
	Vector3f mNormal;
};

