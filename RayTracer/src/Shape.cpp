#include "Shape.h"

Shape::Shape(const Material& LightingMaterial)
	: mMaterial(LightingMaterial)
{

}

void Shape::setMaterial(const Material& NewMaterial)
{ 
	mMaterial = NewMaterial; 
}

Material Shape::getMaterial() const
{ 
	return mMaterial; 
}