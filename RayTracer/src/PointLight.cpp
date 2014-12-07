#include "PointLight.h"

PointLight::PointLight(Color LightColor, Vector3f LightPosition, float MinDistance, float MaxDistance)
	: Light(LightColor)
	, mPosition(LightPosition)
	, mMinRange(MinDistance)
	, mMaxRange(MaxDistance)
{

}

Ray PointLight::GetRayToLight(const Vector3f& surfacePoint)
{
	return Ray(surfacePoint, mPosition - surfacePoint);
}

void PointLight::SetLightPosition(const Vector3f& lightPosition)
{
	mPosition = lightPosition;
}

Vector3f PointLight::GetLightPosition() const
{
	return mPosition;
}

Color PointLight::GetIntesityAt(Vector3f Position) const
{
	float distance = (Position - mPosition).Length();
	if (distance < mMinRange)
		return mLightColor;
	else if (distance < mMaxRange)
		return mLightColor * ((mMaxRange - distance) / (mMaxRange - mMinRange));
	else
		return Color::Black;
}

PointLight::~PointLight()
{
}
