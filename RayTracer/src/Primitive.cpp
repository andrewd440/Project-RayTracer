#include "Primitive.h"

Primitive::Primitive(const Material& LightingMaterial)
	: mMaterial(LightingMaterial)
	, mTransform()
{

}

void Primitive::setMaterial(const Material& NewMaterial)
{ 
	mMaterial = NewMaterial; 
}

Material Primitive::getMaterial() const
{ 
	return mMaterial; 
}

AABB Primitive::getBoundingBox() const
{
	return mBoundingBox;
}

Matrix4 Primitive::GetTransform() const
{
	return mTransform;
}

void Primitive::setBoundingBox(AABB boundingBox)
{
	mBoundingBox = boundingBox;
}

void Primitive::SetTransform(Matrix4 NewTransform)
{
	mTransform = NewTransform;
}