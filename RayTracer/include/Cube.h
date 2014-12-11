#pragma once
#include "Primitive.h"

class FCube : public IPrimitive
{
public:
	/**
	* Construct a renderable 3d cube.
	* @param MinPoint Min bounds of the cube.
	* @param MaxPoint Max bounds of the cube.
	* @LightingMaterial Material of the cube.
	*/
	FCube(Vector3f Center, FMaterial LightingMaterial);
	~FCube();

	/**
	* Checks if a ray intersects the Cube.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the Primitive.
	*/
	bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr) override;

private:
	/**
	* Constructs intersection info at a given point of the cube.
	*/
	void ConstructIntersection(const Vector3f& IntersectionPoint, FIntersection* IntersectionOut);

	/**
	* Construct AABB for KD-tree
	*/
	void ConstructAABB() override;
};
