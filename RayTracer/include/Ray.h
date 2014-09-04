#pragma once
#include "Vector3.h"
#include <limits>

/**
* Class for representing a ray in 3D space.
*/
struct Ray
{
	/**
	* Constructs a ray from an origin point that points in a 
	* given direction.
	* @param OriginiPoint - The origin of the ray
	* @param DirectionVector - A direction for the ray
	*/
	inline Ray(Vector3f OriginPoint, Vector3f DirectionVector)
		: origin(OriginPoint)
		, direction(DirectionVector)
	{
		direction.normalize();
	}

	/**
	* Calculates the point on the ray from the 't' parameter in the
	* ray equation.
	* @param t - Value for t in the ray equation
	* @return Point on the ray at t
	*/
	inline Vector3f getPointAtParameter(float t)
	{
		return origin + direction * t;
	}

	Vector3f origin;		/* Origin or ray */
	Vector3f direction;		/* Direction of ray */
};

