#pragma once
#include "Light.h"
#include "Vector3.h"

class DirectionalLight : public Light
{
public:
	/**
	* Default constructor.
	*/
	DirectionalLight();

	/**
	* Constructs a directional light from a color and direction.
	* @param LightColor - Color of the light
	* @param LightDirection - Direction (Normalized) of the light
	*/
	DirectionalLight(Color LightColor, Vector3f LightDirection);
	
	/**
	* Generate a ray from a given point to the light source.
	* @param SurfacePoint - The destination point for the light
	* @return A ray from the light source pointing to the given surface point.
	*/
	Ray GetRayToLight(const Vector3f& SurfacePoint);

	/**
	* Sets the direction of the light.
 	* @param LightDirection - The direction (Normalized) of the light
	*/
	void setLightDirection(const Vector3f& LightDirection);

	/**
	* Retrieves the direction of the light.
	* @return The direction of the light
	*/
	Vector3f getLightDirection() const;

	/**
	* Retrieves the color intensity of the light at a point. (default light color for
	* directional light)
	* @param Position Point to check intensity at.
	* @return The color of the light.
	*/
	virtual Color GetIntesityAt(Vector3f Position) const;

private:
	Vector3f mLightDirection; /* Direction (Normalized) of the light*/
};

