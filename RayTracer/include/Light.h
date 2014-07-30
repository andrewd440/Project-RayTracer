#pragma once
#include "Color.h"
#include "Vector3.h"

/* Base class of all scene lights */
class Light
{
public:
	/**
	* Default constructor.
	*/
	Light();

	/**
	* Constructs a light with a given color.
	* @param LightColor - The color of the light
	*/
	Light(Color LightColor);

	virtual ~Light();

	/**
	* Generate a direction from a given point to the light source.
	* @param SurfacePoint - The destination point for the light
	* @return A ray from the light source pointing to the given surface point.
	*/
	virtual Vector3f getLightDirectionFromPoint(const Vector3f& SurfacePoint) = 0;

	/**
	* Retrieves the RGB color of the light.
	* @return The color of the light.
	*/
	Color getLightColor() const;

	/**
	* Sets the color of the light.
	* @param LightColor - Color of the light
	*/
	void setLightColor(const Color& LightColor);

private:
	Color mLightColor; /* Color of the light. */
};

