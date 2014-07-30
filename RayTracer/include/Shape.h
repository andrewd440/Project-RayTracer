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
	* Checks if a ray intersects the shape within the range of a t parameter.
	* If the intersection succeeds, the intersection properties are output through
	* a given intersection reference.
	* @param Ray - the ray to check for intersection
	* @param tValueOut - the smallest t parameter will be output to this
	* @param IntersectionOut - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the shape.
	*/
	virtual bool isIntersectingRay(Ray Ray, float& tValueOut, Intersection& IntersectionOut) = 0;

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

