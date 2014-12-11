#include "Primitive.h"

IPrimitive::IPrimitive(const FMaterial& LightingMaterial)
	: mMaterial(LightingMaterial)
	, mTransform()
{

}

void IPrimitive::setMaterial(const FMaterial& NewMaterial)
{ 
	mMaterial = NewMaterial; 
}

FMaterial IPrimitive::getMaterial() const
{ 
	return mMaterial; 
}

AABB IPrimitive::getBoundingBox() const
{
	AABB transformedBox = mBoundingBox;
	transformedBox.max = mTransform.TransformPosition(transformedBox.max);
	transformedBox.min = mTransform.TransformPosition(transformedBox.min);
	return transformedBox;
}

FMatrix4 IPrimitive::GetTransform() const
{
	return mTransform;
}

void IPrimitive::SetTransform(const FMatrix4& NewTransform)
{
	mTransform = NewTransform;
}

void IPrimitive::setBoundingBox(AABB boundingBox)
{
	mBoundingBox = boundingBox;
}
