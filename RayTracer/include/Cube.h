#pragma once
#include "Primitive.h"

class Cube : public Primitive
{
public:
	/**
	* Construct a renderable 3d cube.
	* @param MinPoint Min bounds of the cube.
	* @param MaxPoint Max bounds of the cube.
	* @LightingMaterial Material of the cube.
	*/
	Cube(Vector3f Center, Material LightingMaterial);
	~Cube();

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
	bool IsIntersectingRay(Ray Ray, float* tValueOut = nullptr, Intersection* IntersectionOut = nullptr) const override;

private:
	/**
	* Constructs intersection info at a given point of the cube.
	*/
	void ConstructIntersection(const Vector3f& IntersectionPoint, Intersection* IntersectionOut);

	/**
	* Construct AABB for KD-tree
	*/
	void ConstructAABB() override;
};

