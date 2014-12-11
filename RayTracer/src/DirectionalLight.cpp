#include "DirectionalLight.h"

FDirectionalLight::FDirectionalLight()
	: ILight()
	, mLightDirection(0,-1,0)
{

}

FDirectionalLight::FDirectionalLight(FColor LightColor, Vector3f LightDirection)
	: ILight(LightColor)
	, mLightDirection(LightDirection)
{
	mLightDirection.Normalize();
}


FRay FDirectionalLight::GetRayToLight(const Vector3f& SurfacePoint) const
{
	return FRay(SurfacePoint, -mLightDirection);
}

std::vector<FRay> FDirectionalLight::GetRayToLightSamples(const Vector3f& SurfacePoint, int NumSamples) const
{
	return std::vector<FRay>();
}

void FDirectionalLight::setLightDirection(const Vector3f& LightDirection)
{
	mLightDirection = LightDirection;
	mLightDirection.Normalize();
}

Vector3f FDirectionalLight::getLightDirection() const
{
	return mLightDirection;
}

FColor FDirectionalLight::GetIntesityAt(Vector3f Position) const
{
	Position; // turn off compiler warning
	return mLightColor;
}