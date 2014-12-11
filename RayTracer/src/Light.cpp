#include "Light.h"

ILight::ILight()
	: mLightColor()
{

}

ILight::ILight(FColor LightColor)
	: mLightColor(LightColor)
{

}

ILight::~ILight()
{

}

void ILight::setLightColor(const FColor& LightColor)
{
	mLightColor = LightColor;
}