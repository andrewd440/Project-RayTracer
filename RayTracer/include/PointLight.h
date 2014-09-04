#pragma once
#include "Light.h"

/**
* Represents point light in 3D
*/
class PointLight : public Light
{
public:
	/**
	* Constructs a point light at a specified location.
	* @param lightPosition - 3D position of light
	* @param lightColor - Color of the light
	*/
	PointLight(Color lightColor, Vector3f lightPosition);

	~PointLight();


	/**
	* Generate a ray from a given point to the light source.
	* @param surfacePoint - The destination point for the light
	* @return A ray from the light source pointing to the given surface point.
	*/
	Ray getRayToLight(const Vector3f& surfacePoint);

	/**
	* Sets the position of the light.
	* @param lightPosition - The position of the light
	*/
	void setLightPosition(const Vector3f& lightPosition);

	/**
	* Retrieves the position of the light.
	* @return The position of the light
	*/
	Vector3f getLightPosition() const;

private:
	Vector3f mPosition;
};

