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

void Light::setLightColor(const Color& LightColor)
{
	mLightColor = LightColor;
}