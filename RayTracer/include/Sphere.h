#pragma once
#include "Primitive.h"
#include "Vector3.h"

/**
* Class for representing a sphere in 3D space.
*/
class FSphere : public IPrimitive
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
	* Retrieves the center point of the sphere.
	* @reture 3D coordinates of the center of the sphere
	*/
	Vector3f GetCenter() const;

	/**
	* Sets the center point of the sphere.
	* @param CenterPoint - 3D position of the sphere's center
	*/
	void SetCenter(const Vector3f& CenterPoint);

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

private:
	/**
	* Constructs intersection properties for a point on this sphere
	* @param IntersectionPoint - the point of the intersection
	* @param IntersectionOut - intersection properties will be output through
	*/
	void ConstructIntersection(Vector3f IntersectionPoint, FIntersection& IntersectionOut);

	void ConstructAABB() override;

private:
	float mRadius; /* Length of the radius */
};

