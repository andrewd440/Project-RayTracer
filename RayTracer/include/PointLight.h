#pragma once
#include "Light.h"

/**
* Represents point light in 3D
*/
class FPointLight : public ILight
{
public:
	/**
	* Constructs a point light at a specified location.
	* @param LightColor - Color of the light
	* @param LightPosition - 3D position of light
	* @param SizeRadius - Physical radius of the light object, used to produce multiple
							light samples for soft shadows.
	* @param MinDistance Distance when light fallout begins.
	* @param MaxDistance Distance when light is not visible.
	*/
	FPointLight(FColor LightColor, Vector3f LightPosition, float SizeRadius, float MinDistance, float MaxDistance);

	~FPointLight();

	/**
	* Generate a ray from a given point to the light source.
	* @param surfacePoint - The destination point for the light
	* @return A ray from the light source pointing to the given surface point.
	*/
	FRay GetRayToLight(const Vector3f& SurfacePoint) const override;

	/**
	* Generate a list of sample rays from a given point to an area light source.
	* Used to take shadow samples for generating soft shadows.
	* @param SurfacePoint The destination point for the light
	* @param NumSamples The number of sample to produce
	* @return A list of sample rays from the surface point to the light position.
	*/
	std::vector<FRay> GetRayToLightSamples(const Vector3f& SurfacePoint, int NumSamples) const override;

	/**
	* Sets the position of the light.
	* @param lightPosition - The position of the light
	*/
	void SetLightPosition(const Vector3f& LightPosition);

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
	FColor GetIntesityAt(Vector3f Position) const override;

private:
	Vector3f mPosition;
	float mSizeRadius; /* Physical radius of the light object */

	/* Falloff for linear light attenuation */
	float mMinRange;
	float mMaxRange;
};

