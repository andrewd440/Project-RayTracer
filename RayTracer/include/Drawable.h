#pragma once

#include "Object.h"
#include "Material.h"
#include "AABB.h"
#include "Matrix4.h"

class FTexture;

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
	* Set the default material properties for the Primitives' surface.
	* @param NewMaterial - The material for the Primitive
	*/
	virtual void SetMaterial(const FMaterial& NewMaterial);

	/**
	* Retrieves the material for the Primitive at a point in world space.
	* @return The material
	*/
	virtual FMaterial GetMaterial(Vector3f SurfacePoint) = 0;

	/**
	* Retrieves the bounding box for the Primitive.
	*/
	AABB GetBoundingBox() const;

	/**
	* Retrieves the diffuse texture on the current object.
	*/
	FTexture* GetTexture() const;

	/**
	* Sets a diffuse texture on the current object.
	*/
	void SetTexture(FTexture& Texture);

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
	* Set the origin of the object.
	*/
	void SetOrigin(const Vector3f& Origin);

	/**
	* Get the origin of the object.
	*/
	Vector3f GetOrigin() const;

protected:
	/**
	* Sets the bounding box for the Primitive.
	* @param boundingBox - AABB for the Primitive
	*/
	void SetBoundingBox(AABB boundingBox);

protected:
	FMaterial mMaterial;			/* Lighting material properties for the Primitive */
	FTexture* mDiffuseTexture;		/* Mapped diffise texture for the object */
	IDrawable* mParentObject;		/* Parent of this object */

private:
	/**
	* Each derived class needs to construct their bounding box.
	*/
	virtual void ConstructAABB(Vector3f Min = Vector3f(), Vector3f Max = Vector3f()) = 0;

private:
	AABB mBoundingBox;				/* Bounding volume */
	FMatrix4 mTransform;			/* Object space transform */
	FMatrix4 mInvTransform;			/* Inverse transform of this object, takes object from world to object space */
};

