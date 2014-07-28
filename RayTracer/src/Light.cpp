#include "Light.h"

Light::Light()
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