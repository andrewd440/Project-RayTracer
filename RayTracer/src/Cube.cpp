#include "Cube.h"
#include "Intersection.h"
#include <limits>


FCube::FCube(Vector3f Center, FMaterial LightingMaterial)
	: IPrimitive(LightingMaterial)
{
	mTransform.SetOrigin(Center);
	ConstructAABB();
}

bool FCube::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	/**
	* Ray-AABB intersection test from Real-Time Collision Detection by Christer Ericson p. 181
	* Test checks AABB slabs against ray for concurrent intersection with each slab
	**/

	float tMin = 0.0f;
	float tMax = (tValueOut) ? *tValueOut : std::numeric_limits<float>::max();

	const Vector3f boxMin(mTransform.TransformPosition(Vector3f(-1, -1, 1)));
	const Vector3f boxMax(mTransform.TransformPosition(Vector3f(1, 1, -1)));

	const Vector3f& rayD = Ray.direction;
	const Vector3f& rayO = Ray.origin;
	
	for (int i = 0; i < 3; i++)
	{
		if (abs(rayD[i]) < _EPSILON)
		{
			//Ray is parallel to this slab, no hit unless origin is within slab
			if (rayO[i] < boxMin[i] || rayO[i] > boxMax[i])
				return false;
		}
		else
		{
			// Get intersection of t value with near and far planes of slab
			float ood = 1.0f / rayD[i];
			float t1 = (boxMin[i] - rayO[i]) * ood;
			float t2 = (boxMax[i] - rayO[i]) * ood;

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
		ConstructIntersection(rayO + tMin * rayD, IntersectionOut);
	}

	return true;
}

void FCube::ConstructIntersection(const Vector3f& IntersectionPoint, FIntersection* IntersectionOut)
{
	float largestSide = 0;
	int intersectionAxis = 0;
	int intersectionSide = 0;

	const FMatrix4& modelInverse = mTransform.GetInverse();

	const Vector3f intersectionInObjectSpace = modelInverse.TransformPosition(IntersectionPoint);

	// get the intersection plane
	for (int i = 0; i < 3; i++)
	{
		// get axis with largest length
		if (abs(intersectionInObjectSpace[i]) > largestSide)
		{
			largestSide = abs(intersectionInObjectSpace[i]);
			intersectionSide = (intersectionInObjectSpace[i] < 0.0f) ? -1 : 1;
			intersectionAxis = i;
		}
	}

	Vector3f normal;
	normal[intersectionAxis] = (float)intersectionSide;
	normal = modelInverse.TransformDirection(normal).Normalize();

	IntersectionOut->normal = normal;
	IntersectionOut->object = this;

	// make sure point not inside of the object
	IntersectionOut->point = IntersectionPoint + (normal * _EPSILON);
}

void FCube::ConstructAABB()
{
	Vector3f max(1, 1, -1);
	Vector3f min(-1, -1, 1);

	setBoundingBox(AABB{min, max});
}

FCube::~FCube()
{
}
