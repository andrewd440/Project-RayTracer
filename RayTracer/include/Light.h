#pragma once
#include "Color.h"
#include "Vector3.h"
#include "Ray.h"

#include <vector>

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
	* Generate a list of sample rays from a given point to an area light source.
	* Used to take shadow samples for generating soft shadows.
	* @param SurfacePoint The destination point for the light
	* @param NumSamples The number of sample to produce
	* @return A list of sample rays from the surface point to the light position.
	*/
	virtual std::vector<FRay> GetRayToLightSamples(const Vector3f& SurfacePoint, int NumSamples) const = 0;

	/**
	* Retrieves the color intensity of the light at a point.
	* @param Position Point to check intensity at.
	* @return The color of the light.
	*/
	virtual FColor GetIntesityAt(Vector3f Position) const = 0;

	/**
	* Get the distance from a world point to the light source.
	*/
	virtual float GetDistance(const Vector3f Position) const = 0;

	/**
	* Sets the color of the light.
	* @param LightColor - Color of the light
	*/
	void SetLightColor(const FColor& LightColor);

protected:
	FColor mLightColor; /* Color of the light. */
};

