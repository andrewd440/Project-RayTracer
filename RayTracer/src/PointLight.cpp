#include "PointLight.h"

PointLight::PointLight(Color lightColor, Vector3f lightPosition)
	: Light(lightColor)
	, mPosition(lightPosition)
{

}

Ray PointLight::getRayToLight(const Vector3f& surfacePoint)
{
	return Ray(mPosition, mPosition - surfacePoint);
}

void PointLight::setLightPosition(const Vector3f& lightPosition)
{
	mPosition = lightPosition;
}

Vector3f PointLight::getLightPosition() const
{
	return mPosition;
}


PointLight::~PointLight()
{
}
