#pragma once
#include "Ray.h"
#include "Material.h"
#include "AABB.h"
#include "Matrix4.h"

struct Intersection;

/**
* Abstract class for all Primitives.
*/
class Primitive
{
public:
	explicit Primitive(const Material& LightingMaterial);

	virtual ~Primitive(){};

	/**
	* Checks if a ray intersects the Primitive.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the Primitive.
	*/
	virtual bool IsIntersectingRay(Ray Ray, float* tValueOut = nullptr, Intersection* IntersectionOut = nullptr) = 0;

	/**
	* Set the material properties for the Primitives' surface.
	* @param NewMaterial - The material for the Primitive
	*/
	void setMaterial(const Material& NewMaterial);

	/**
	* Retrieves the material for the Primitive.
	* @return The material
	*/
	Material getMaterial() const;

	/**
	* Retrieves the bounding box for the Primitive.
	*/
	AABB getBoundingBox() const;

	/**
	* Gets the model transform for the object.
	*/
	Matrix4 GetTransform() const;

protected:
	/**
	* Sets the bounding box for the Primitive.
	* @param boundingBox - AABB for the Primitive
	*/
	void setBoundingBox(AABB boundingBox);

protected:
	Matrix4 mTransform; /* Object space transform */

private:
	/**
	* Each derived class needs to construct their bounding box.
	*/
	virtual void ConstructAABB() = 0;

private:
	Material mMaterial; /* Lighting material properties for the Primitive */
	AABB mBoundingBox; /* AABB for KD-tree */
};

