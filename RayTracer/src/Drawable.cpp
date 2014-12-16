#include "Drawable.h"

IDrawable::IDrawable(const FMaterial& LightingMaterial)
	: mMaterial(LightingMaterial)
	, Transform()
{

}

void IDrawable::SetMaterial(const FMaterial& NewMaterial)
{ 
	mMaterial = NewMaterial; 
}

FMaterial IDrawable::GetMaterial() const
{ 
	return mMaterial; 
}

AABB IDrawable::GetBoundingBox() const
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

void IDrawable::SetBoundingBox(AABB boundingBox)
{
	mBoundingBox = boundingBox;
}
