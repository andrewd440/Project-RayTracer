#pragma once
#include "Ray.h"

struct Intersection;

/**
* Abstract class for all shapes.
*/
class Shape
{
public:

	virtual ~Shape(){};

	/**
	* Checks if a ray intersects the shape within the range of a t parameter.
	* If the intersection succeeds, the intersection properties are output through
	* a given intersection reference.
	* @param ray - the ray to check for intersection
	* @param tMax - the max value for the t parameter or the ray (distance of the ray)
	* @param intersectionOut - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the shape.
	*/
	virtual bool isIntersectingRay(Ray ray, float tMax, Intersection& intersectionOut) = 0;
};

