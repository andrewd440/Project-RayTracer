#include "Drawable.h"

IDrawable::IDrawable(const FMaterial& LightingMaterial)
	: mTransform()
	, mMaterial(LightingMaterial)
	, mParentObject(nullptr)
	, mBoundingBox()
	, mInvTransform()
	, mIsEnabled(true)
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

void IDrawable::SetLocalOrigin(const Vector3f& Origin)
{
	mTransform.SetOrigin(Origin);
	mInvTransform = mTransform.GetInverse();
}

Vector3f IDrawable::GetLocalOrigin() const
{
	return mTransform.GetOrigin();
}

Vector3f IDrawable::GetWorldOrigin() const
{
	if (mParentObject)
		return mParentObject->GetWorldOrigin() + mTransform.GetOrigin();

	return mTransform.GetOrigin();
}

bool IDrawable::IsEnabled() const
{
	return mIsEnabled;
}

void IDrawable::SetEnabled(bool IsEnabled)
{
	mIsEnabled = IsEnabled;
}