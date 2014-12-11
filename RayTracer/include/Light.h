#pragma once
#include "Color.h"
#include "Vector3.h"
#include "Ray.h"

/* Base class of all scene lights */
class ILight
{
public:
	/**
	* Default constructor.
	*/
	ILight();

	/**
	* Constructs a light with a given color.
	* @param LightColor - The color of the light
	*/
	ILight(FColor LightColor);

	virtual ~ILight();

	/**
	* Generate a ray from a given point to the light source.
	* @param SurfacePoint - The destination point for the light
	* @return A ray from the surface point to the light position.
	*/
	virtual FRay GetRayToLight(const Vector3f& SurfacePoint) const = 0;

	/**
	* Retrieves the color intensity of the light at a point.
	* @param Position Point to check intensity at.
	* @return The color of the light.
	*/
	virtual FColor GetIntesityAt(Vector3f Position) const = 0;

	/**
	* Sets the color of the light.
	* @param LightColor - Color of the light
	*/
	void setLightColor(const FColor& LightColor);

protected:
	FColor mLightColor; /* Color of the light. */
};

