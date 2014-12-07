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
	* @param LightColor - Color of the light
	* @param LightPosition - 3D position of light
	* @param MinDistance Distance when light fallout begins.
	* @param MaxDistance Distance when light is not visible.
	*/
	PointLight(Color LightColor, Vector3f LightPosition, float MinDistance, float MaxDistance);

	~PointLight();

	/**
	* Generate a ray from a given point to the light source.
	* @param surfacePoint - The destination point for the light
	* @return A ray from the light source pointing to the given surface point.
	*/
	Ray GetRayToLight(const Vector3f& surfacePoint);

	/**
	* Sets the position of the light.
	* @param lightPosition - The position of the light
	*/
	void SetLightPosition(const Vector3f& lightPosition);

	/**
	* Retrieves the position of the light.
	* @return The position of the light
	*/
	Vector3f GetLightPosition() const;

	/**
	* Retrieves the color intensity of the light at a point.
	* @param Position Point to check intensity at.
	* @return The color of the light.
	*/
	virtual Color GetIntesityAt(Vector3f Position) const;

private:
	Vector3f mPosition;

	/* Falloff for linear light attenuation */
	float mMinRange;
	float mMaxRange;
};

