#pragma once
#include "Vector3.h"
#include <limits>

/**
* Class for representing a ray in 3D space.
*/
class Ray
{
	/**
	* Constructs a ray from an origin point that points in a 
	* given direction.
	* @param originiPoint - the origin of the ray
	* @param directionVector - a normalized direction for the ray
	*/
	inline Ray(Vector3f originPoint, Vector3f directionVector)
	{
		mOriginAndDirection[0] = originPoint;
		mOriginAndDirection[1] = directionVector;
	}

	/**
	* Calculates the point on the ray from the 't' parameter in the
	* ray equation.
	* @param t - Value for t in the ray equation
	* @return Point on the ray at t
	*/
	inline Vector3f getPointAtParameter(float t)
	{
		return mOriginAndDirection[0] + mOriginAndDirection[1] * t;
	}

	/**
	* Retrieves the origin of the ray.
	* @return Origin point of the ray.
	*/
	inline Vector3f getOrigin() const
	{
		return mOriginAndDirection[0];
	}

	/**
	* Retrieves the direction of the ray.
	* @return Normalized direction vector
	*/
	inline Vector3f getDirection() const
	{
		return mOriginAndDirection[1];
	}

private:
	/** 
	* Holds origin at index 0 and direction at index 1.
	* Since these are accessed together frequently, we
	* keep them together in memory.
	*/
	Vector3f mOriginAndDirection[2];
};

