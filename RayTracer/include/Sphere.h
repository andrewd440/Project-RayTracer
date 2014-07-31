#pragma once
#include "Shape.h"
#include "Vector3.h"

/**
* Class for representing a sphere in 3D space.
*/
class Sphere : public Shape
{
public:
	/**
	* Constructs a sphere from a center point and radius
	* @param Center - The center of the sphere
	* @param Radius - Length of the radius
	* @param LightMaterial - Lighting properties for the sphere
	*/
	Sphere(Vector3f Center, float Radius, Material LightingMaterial);
	
	/**
	* Checks if a ray intersects the sphere within the range of a t parameter.
	* If the intersection succeeds, the intersection properties are output through
	* a given intersection reference.
	* @param Ray - the ray to check for intersection
	* @param tValueOut - the smallest t parameter will be output to this if it is smaller than
	*						the value already in tValueOut
	* @param IntersectionOut - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the shape.
	*/
	virtual bool isIntersectingRay(Ray Ray, float& tValueOut, Intersection& IntersectionOut);

	/**
	* Checks if a ray intersects the shape.
	* @param Ray - the ray to check for intersection
	* @return True if the ray intersects the shape.
	*/
	virtual bool isIntersectingRay(Ray Ray);

	/**
	* Retrieves the center point of the sphere.
	* @reture 3D coordinates of the center of the sphere
	*/
	Vector3f getCenter() const;

	/**
	* Sets the center point of the sphere.
	* @param CenterPoint - 3D position of the sphere's center
	*/
	void setCenter(const Vector3f& CenterPoint);

	/**
	* Retrieves the radius of the sphere.
	* @return The length of the radius
	*/
	float getRadius() const;

	/**
	* Sets the radius of the sphere.
	* @param Radius - length of the radius
	*/
	void setRadius(const float& Radius);

private:
	/**
	* Constructs intersection properties for a point on this sphere
	* @param IntersectionPoint - the point of the intersection
	* @param IntersectionOut - intersection properties will be output through
	*/
	void constructIntersection(Vector3f IntersectionPoint, Intersection& IntersectionOut);

private:
	Vector3f mCenter; /* Coordinates of the center of the sphere */
	float mRadius; /* Length of the radius */
};

