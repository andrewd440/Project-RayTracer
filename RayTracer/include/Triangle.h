#pragma once
#include "Drawable.h"
#include "Vector3.h"

class FTriangle : public IDrawable
{
public:
	/* Default constructor */
	FTriangle();

	/**
	* Constructs a triangle from three vertices, normal is computed on construction.
	* @param V0 First vertex
	* @param V1 Second vertex
	* @param V2 Third vertex
	* @param LightingMaterial for the triangle.
	*/
	FTriangle(Vector3f V0, Vector3f V1, Vector3f V2, const FMaterial& LightingMaterial = FMaterial());

	/**
	* Constructs a triangle from three vertices, a normal, and lighting material.
	* @param V0 First vertex
	* @param V1 Second vertex
	* @param V2 Third vertex
	* @param Normal of the triangle
	* @param LightingMaterial for the triangle.
	*/
	FTriangle(Vector3f V0, Vector3f V1, Vector3f V2, Vector3f Normal, const FMaterial& LightingMaterial = FMaterial());
	
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
	bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr) override;


private:
	/**
	* Constructs intersection properties for a point on this triangle
	* @param IntersectionPoint - the point of the intersection
	* @param IntersectionOut - intersection properties will be output through
	*/
	void ConstructIntersection(Vector3f IntersectionPoint, FIntersection* IntersectionOut);

	void ConstructAABB(Vector3f Min = Vector3f(), Vector3f Max = Vector3f()) override;

private:
	Vector3f mV0;
	Vector3f mV1;
	Vector3f mV2;
	Vector3f mNormal;
};

