#pragma once
#include "Shape.h"

/* Represents a plane in 3D space. */
class Plane : public Shape
{
public:
	/**
	* Constructs a plane from a specified normal and point that is on the plane.
	* @param PlaneNormal - The normal for the plane.
	* @param PointOnPlane - A point on the plane.
	*/
	Plane(const Material& LightingMaterial, Vector3f PlaneNormal, Vector3f PointOnPlane);
	
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
	bool isIntersectingRay(Ray Ray, float* tValueOut = nullptr, Intersection* IntersectionOut = nullptr);


private:
	/**
	* Constructs intersection properties for a point on this plane
	* @param IntersectionPoint - the point of the intersection
	* @param IntersectionOut - intersection properties will be output through
	*/
	void constructIntersection(Vector3f IntersectionPoint, Intersection& IntersectionOut);

private:
	Vector3f mNormal;
	float mDistanceFromOrigin;
};

