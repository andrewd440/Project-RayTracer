#pragma once
#include "Ray.h"

struct FIntersection;

/**
* Base class for all objects that can interact with
* rays used to generate the scene.
*/
class IObject
{
public:

	IObject(){}

	virtual ~IObject(){}

	/**
	* Checks if a ray intersects the object.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the Primitive.
	*/
	virtual bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr) = 0;
};

