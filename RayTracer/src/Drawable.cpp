#include "Drawable.h"

IDrawable::IDrawable(const FMaterial& LightingMaterial)
	: mTransform()
	, mMaterial(LightingMaterial)
	, mDiffuseTexture(nullptr)
	, mParentObject(nullptr)
	, mBoundingBox()
	, mInvTransform()
{

}

void IDrawable::SetMaterial(const FMaterial& NewMaterial)
{ 
	mMaterial = NewMaterial; 
}

AABB IDrawable::GetBoundingBox() const
{
	return mBoundingBox;
}

FTexture* IDrawable::GetTexture() const
{
	return mDiffuseTexture;
}

void IDrawable::SetTexture(FTexture* Texture)
{
	mDiffuseTexture = Texture;
}

void IDrawable::SetBoundingBox(AABB boundingBox)
{
	mBoundingBox = boundingBox;
}

void IDrawable::SetParent(IDrawable& Parent)
{
	mParentObject = &Parent;
}

FMatrix4 IDrawable::GetInvTransform() const
{
	return mInvTransform;
}

FMatrix4 IDrawable::GetWorldInvTransform() const
{
	if (mParentObject)
		return mParentObject->GetWorldInvTransform() * mInvTransform;

	return mInvTransform;
}

FMatrix4 IDrawable::GetTransform() const
{
	return mTransform;
}

FMatrix4 IDrawable::GetWorldTransform() const
{
	if (mParentObject)
		return mParentObject->GetWorldTransform() * mTransform;

	return mTransform;
}

void IDrawable::SetTransform(const FMatrix4& Transform)
{
	mTransform = Transform;
	mInvTransform = Transform.GetInverse();
}

void IDrawable::Rotate(EAxis Axis, float Degrees)
{
	mTransform.Rotate(Axis, Degrees);
	mInvTransform = mTransform.GetInverse();
}

void IDrawable::Scale(EAxis Axis, float Scale)
{
	mTransform.Scale(Axis, Scale);
	mInvTransform = mTransform.GetInverse();
}

void IDrawable::SetOrigin(const Vector3f& Origin)
{
	mTransform.SetOrigin(Origin);
	mInvTransform = mTransform.GetInverse();
}

Vector3f IDrawable::GetOrigin() const
{
	return mTransform.GetOrigin();
}