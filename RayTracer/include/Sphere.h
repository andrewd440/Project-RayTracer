#pragma once
#include "Drawable.h"
#include "Vector3.h"

/**
* Class for representing a sphere in 3D space.
*/
class FSphere : public IDrawable
{
public:
	/**
	* Constructs a sphere from a center point and radius
	* @param Center - The center of the sphere
	* @param Radius - Length of the radius
	* @param LightMaterial - Lighting properties for the sphere
	*/
	FSphere(Vector3f Center, float Radius, FMaterial LightingMaterial);
	
	/**
	* Checks if a ray intersects the sphere.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the sphere.
	*/
	bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr) override;

	/**
	* Retrieves the radius of the sphere.
	* @return The length of the radius
	*/
	float GetRadius() const;

	/**
	* Sets the radius of the sphere.
	* @param Radius - length of the radius
	*/
	void SetRadius(const float& Radius);

	/**
	* Retrieves the material for the sphere.
	* @return The material
	*/
	FMaterial GetMaterial(Vector3f SurfacePoint) override;

private:
	/**
	* Constructs intersection properties for a point on this sphere
	* @param IntersectionPoint - the point of the intersection
	* @param IntersectionOut - intersection properties will be output through
	*/
	void ConstructIntersection(Vector3f IntersectionPoint, FIntersection& IntersectionOut);

	void ConstructAABB(Vector3f Min = Vector3f(), Vector3f Max = Vector3f()) override;

private:
	float mRadius; /* Length of the radius */
};

