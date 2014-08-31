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

AABB Shape::getBoundingBox() const
{
	return mBoundingBox;
}

void Shape::setBoundingBox(AABB boundingBox)
{
	mBoundingBox = boundingBox;
}