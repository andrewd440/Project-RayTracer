#pragma once
#include "Shape.h"
#include "Vector3.h"

class Triangle : public Shape
{
public:
	Triangle(Vector3f V1, Vector3f V2, Vector3f V3, const Material& LightingMaterial);
	
	/**
	* Checks if a ray intersects the triangle.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the triangle.
	*/
	bool isIntersectingRay(Ray Ray, float* tValueOut = nullptr, Intersection* IntersectionOut = nullptr);

private:
	Vector3f mVertex1;
	Vector3f mVertex2;
	Vector3f mVertex3;
	Vector3f mNormal;
};

