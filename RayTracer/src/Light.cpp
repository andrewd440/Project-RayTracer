#include "Light.h"

Light::Light()
	: mLightColor()
{

}

Light::Light(Color LightColor)
	: mLightColor(LightColor)
{

}

Light::~Light()
{

}

Color Light::getLightColor() const
{
	return mLightColor;
}

void Light::setLightColor(const Color& LightColor)
{
	mLightColor = LightColor;
}