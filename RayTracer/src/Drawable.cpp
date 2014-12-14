#include "Drawable.h"

IDrawable::IDrawable(const FMaterial& LightingMaterial)
	: mMaterial(LightingMaterial)
	, Transform()
{

}

void IDrawable::setMaterial(const FMaterial& NewMaterial)
{ 
	mMaterial = NewMaterial; 
}

FMaterial IDrawable::getMaterial() const
{ 
	return mMaterial; 
}

AABB IDrawable::getBoundingBox() const
{
	return mBoundingBox;
}

//FMatrix4 IPrimitive::GetTransform() const
//{
//	return mTransform;
//}
//
//void IPrimitive::SetTransform(const FMatrix4& NewTransform)
//{
//	mTransform = NewTransform;
//}

void IDrawable::setBoundingBox(AABB boundingBox)
{
	mBoundingBox = boundingBox;
}
