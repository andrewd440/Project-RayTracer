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
	AABB transformedBox = mBoundingBox;
	transformedBox.max = mTransform.TransformPosition(transformedBox.max);
	transformedBox.min = mTransform.TransformPosition(transformedBox.min);
	return transformedBox;
}

Matrix4 Primitive::GetTransform() const
{
	return mTransform;
}

void Primitive::setBoundingBox(AABB boundingBox)
{
	mBoundingBox = boundingBox;
}
