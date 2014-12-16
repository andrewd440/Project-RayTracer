#pragma once

#include "Object.h"
#include "Material.h"
#include "AABB.h"
#include "Matrix4.h"


/**
* Abstract class for all Primitives.
*/
class IDrawable : public IObject
{
public:
	/**
	* Default constructor.
	* Constructs a drawable object with an optional lighting material.
	*/
	explicit IDrawable(const FMaterial& LightingMaterial = FMaterial());

	virtual ~IDrawable(){};

	/**
	* Checks if a ray intersects the primitive.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the Primitive.
	*/
	virtual bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr) = 0;

	/**
	* Set the material properties for the Primitives' surface.
	* @param NewMaterial - The material for the Primitive
	*/
	virtual void SetMaterial(const FMaterial& NewMaterial);

	/**
	* Retrieves the material for the Primitive.
	* @return The material
	*/
	FMaterial GetMaterial() const;

	/**
	* Retrieves the bounding box for the Primitive.
	*/
	AABB GetBoundingBox() const;

public:
	FMatrix4 Transform; /* Object space transform */

protected:
	/**
	* Sets the bounding box for the Primitive.
	* @param boundingBox - AABB for the Primitive
	*/
	void SetBoundingBox(AABB boundingBox);

private:
	/**
	* Each derived class needs to construct their bounding box.
	*/
	virtual void ConstructAABB(Vector3f Min = Vector3f(), Vector3f Max = Vector3f()) = 0;

private:
	FMaterial mMaterial; /* Lighting material properties for the Primitive */
	AABB mBoundingBox; /* Bounding volume */
};

