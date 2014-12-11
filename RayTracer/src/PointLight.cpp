#include "PointLight.h"

FPointLight::FPointLight(FColor LightColor, Vector3f LightPosition, float MinDistance, float MaxDistance)
	: ILight(LightColor)
	, mPosition(LightPosition)
	, mMinRange(MinDistance)
	, mMaxRange(MaxDistance)
{

}

FRay FPointLight::GetRayToLight(const Vector3f& surfacePoint) const
{
	return FRay(surfacePoint, mPosition - surfacePoint);
}

void FPointLight::SetLightPosition(const Vector3f& lightPosition)
{
	mPosition = lightPosition;
}

Vector3f FPointLight::GetLightPosition() const
{
	return mPosition;
}

FColor FPointLight::GetIntesityAt(Vector3f Position) const
{
	float distance = (Position - mPosition).Length();
	if (distance < mMinRange)
		return mLightColor;
	else if (distance < mMaxRange)
		return mLightColor * ((mMaxRange - distance) / (mMaxRange - mMinRange));
	else
		return FColor::Black;
}

FPointLight::~FPointLight()
{
}
