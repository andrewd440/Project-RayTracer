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
	* @param LightDirection - Direction (normalized) of the light
	*/
	DirectionalLight(Color LightColor, Vector3f LightDirection);
	
	/**
	* Generate a direction from a given point to the light source.
	* @param SurfacePoint - The destination point for the light
	* @return A ray from the light source pointing to the given surface point.
	*/
	Vector3f getLightDirectionFromPoint(const Vector3f& SurfacePoint);

	/**
	* Sets the direction of the light.
 	* @param LightDirection - The direction (normalized) of the light
	*/
	void setLightDirection(const Vector3f& LightDirection);

	/**
	* Retrieves the direction of the light.
	* @return The direction of the light
	*/
	Vector3f getLightDirection() const;

private:
	Vector3f mLightDirection; /* Direction (normalized) of the light*/
};

