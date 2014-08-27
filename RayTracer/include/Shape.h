#pragma once
#include "Ray.h"
#include "Material.h"

struct Intersection;

/**
* Abstract class for all shapes.
*/
class Shape
{
public:
	Shape(const Material& LightingMaterial);

	virtual ~Shape(){};

	/**
	* Checks if a ray intersects the shape.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the shape.
	*/
	virtual bool isIntersectingRay(Ray Ray, float* tValueOut = nullptr, Intersection* IntersectionOut = nullptr) = 0;

	/**
	* Set the material properties for the shapes' surface.
	* @param NewMaterial - The material for the shape
	*/
	void setMaterial(const Material& NewMaterial);

	/**
	* Retrieves the material for the shape.
	* @return The material
	*/
	Material getMaterial() const;

private:
	Material mMaterial; /* Lighting material properties for the shape */
};

