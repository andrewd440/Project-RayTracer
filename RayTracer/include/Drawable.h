#pragma once

#include "Material.h"
#include "AABB.h"
#include "Matrix4.h"
#include "Ray.h"

class FTexture;
struct FIntersection;

/**
* Abstract class for all renderable objects.
* Each derived class must construct it's own
* AABB for use with the Kd-tree.
*/
class IDrawable
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
	* Set the default material properties for the Primitives' surface.
	* @param NewMaterial - The material for the Primitive
	*/
	virtual void SetMaterial(const FMaterial& NewMaterial);

	/**
	* Retrieves the default material for the Primitive.
	* @return The material
	*/
	FMaterial GetMaterial() const;

	/**
	* Retrieves the material for the Primitive at a point in world space.
	* @return The material
	*/
	virtual FMaterial GetMaterial(Vector3f SurfacePoint) = 0;

	/**
	* Retrieves the bounding box for the Primitive in object space.
	*/
	AABB GetBoundingBox() const;

	/**
	* Retrieves the AABB of the Primitive in world space.
	*/
	AABB GetWorldAABB() const;

	/**
	* Sets the parent transform for this object.
	*/
	void SetParent(IDrawable& Parent);

	/**
	* Gets the inverse transform of the object.
	*/
	FMatrix4 GetInvTransform() const;

	/**
	* Gets the world inverse transform of the object. Includes
	* parent transforms, if any.
	*/
	FMatrix4 GetWorldInvTransform() const;

	/**
	* Gets the transform of the object.
	*/
	FMatrix4 GetTransform() const;

	/**
	* Gets the world transform of the object. Includes
	* parent transforms, if any.
	*/
	FMatrix4 GetWorldTransform() const;

	/**
	* Sets the transform for the object.
	*/
	void SetTransform(const FMatrix4& Transform);

	/**
	* Rotate the object about an axis.
	*/
	void Rotate(EAxis Axis, float Degrees);

	/**
	* Scale the object about an axis.
	*/
	void Scale(EAxis Axis, float Scale);

	/**
	* Set the local origin of the object,
	* in respect of it's parent, if any.
	*/
	void SetLocalOrigin(const Vector3f& Origin);

	/**
	* Get the origin of the object, in respect
	* to it's parent, if any.
	*/
	Vector3f GetLocalOrigin() const;

	/**
	* Get the absolute world position of the object.
	*/
	Vector3f GetWorldOrigin() const;

	/**
	* Check if this object is enabled. Disabled objects
	* do not intersect other objects.
	*/
	bool IsEnabled() const;

	/**
	* Set status of object. Disabled objects do not intersect
	* other objects.
	*/
	void SetEnabled(bool IsEnabled);

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


protected:
	FMaterial mMaterial;			/* Lighting material properties for the Primitive */
	IDrawable* mParentObject;		/* Parent of this object */

private:
	AABB mBoundingBox;				/* Bounding volume */
	FMatrix4 mTransform;			/* Object space transform */
	FMatrix4 mInvTransform;			/* Inverse transform of this object, takes object from world to object space */
	bool mIsEnabled;				/* If the object is disabled, no intersections will take place on it */
};

