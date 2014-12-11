#pragma once
#include "Primitive.h"

/* Represents a plane in 3D space. */
class FPlane : public IPrimitive
{
public:
	/**
	* Constructs a plane from a specified normal and point that is on the plane.
	* @param PlaneNormal - The normal for the plane.
	* @param PointOnPlane - A point on the plane.
	*/
	FPlane(const FMaterial& LightingMaterial, Vector3f PlaneNormal, Vector3f PointOnPlane);
	
	/**
	* Checks if a ray intersects the plane.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the plane.
	*/
	bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr) override;


private:
	/**
	* Constructs intersection properties for a point on this plane
	* @param IntersectionPoint - the point of the intersection
	* @param IntersectionOut - intersection properties will be output through
	*/
	void ConstructIntersection(Vector3f IntersectionPoint, FIntersection& IntersectionOut);

	void ConstructAABB() override;

private:
	Vector3f mNormal;
	float mDistanceFromOrigin;
};

