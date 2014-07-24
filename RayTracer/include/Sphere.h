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
	* @param center - the center of the sphere
	* @param radius - length of the radius
	*/
	Sphere(Vector3f center, float radius);
	
	/**
	* Checks if a ray intersects the sphere within the range of a t parameter.
	* If the intersection succeeds, the intersection properties are output through
	* a given intersection reference.
	* @param ray - the ray to check for intersection
	* @param tMax - the max value for the t parameter or the ray (distance of the ray)
	* @param intersectionOut - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the shape.
	*/
	bool isIntersectingRay(Ray ray, float tMax, Intersection& intersectionOut);

	/**
	* Retrieves the center point of the sphere.
	* @reture 3D coordinates of the center of the sphere
	*/
	Vector3f getCenter() const;

	/**
	* Sets the center point of the sphere.
	* @param center - 3D position of the sphere's center
	*/
	void setCenter(const Vector3f& center);

	/**
	* Retrieves the radius of the sphere.
	* @return The length of the radius
	*/
	float getRadius() const;

	/**
	* Sets the radius of the sphere.
	* @param radius - length of the radius
	*/
	void setRadius(const float& radius);

private:
	/**
	* Constructs intersection properties for a point on this sphere
	* @param point - the point of the intersection
	* @param intersectionOut - intersection properties will be output through
	*/
	void constructIntersection(Vector3f point, Intersection& intersectionOut);

private:
	Vector3f mCenter; /* Coordinates of the center of the sphere */
	float mRadius; /* Length of the radius */
};

