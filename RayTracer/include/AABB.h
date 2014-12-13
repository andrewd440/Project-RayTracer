#pragma once

#include "Object.h"
#include "FMath.h"

/**
* Bounding volume used to accelerate intersection tests.
*/
class AABB : public IObject
{
public:
	Vector3f Min, Max;

	AABB(Vector3f MinBounds = Vector3f(), Vector3f MaxBounds = Vector3f());

	/**
	* Checks if a ray intersects the AABB.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the Primitive.
	*/
	bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr) override;

	Vector3f GetCenter() const;
};


inline AABB::AABB(Vector3f MinBounds, Vector3f MaxBounds)
	: Min(MinBounds)
	, Max(MaxBounds)
{
}

inline bool AABB::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	/**
	* Ray-AABB intersection test from Real-Time Collision Detection by Christer Ericson p. 181
	* Test checks AABB slabs against ray for concurrent intersection with each slab
	**/

	float tMin = 0.0f;
	float tMax = (tValueOut) ? *tValueOut : std::numeric_limits<float>::max();

	const Vector3f& RayD = Ray.direction;
	const Vector3f& RayO = Ray.origin;

	for (int i = 0; i < 3; i++)
	{
		if (abs(RayD[i]) < _EPSILON)
		{
			//Ray is parallel to this slab, no hit unless origin is within slab
			if (RayO[i] < Min[i] || RayO[i] > Max[i])
				return false;
		}
		else
		{
			// Get intersection of t value with near and far planes of slab
			float ood = 1.0f / RayD[i];
			float t1 = (Min[i] - RayO[i]) * ood;
			float t2 = (Max[i] - RayO[i]) * ood;

			// Make t1 be the near plane
			if (t1 > t2)
				std::swap(t1, t2);

			// Compute intersection of slab intersection intervals
			tMin = std::max(tMin, t1);
			tMax = std::min(tMax, t2);

			// Exit if no collision when slab intersection becomes empty
			if (tMin > tMax)
				return false;
		}
	}

	// Ray intersects all slabs, return nearest t value if less than tValueOut
	if (tValueOut && tMin < *tValueOut)
	{
		*tValueOut = tMin;
	}

	return true;
}

inline Vector3f AABB::GetCenter() const
{
	return (Min + Max) * 0.5f;
}