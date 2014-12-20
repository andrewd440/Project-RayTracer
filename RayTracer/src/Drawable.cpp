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


AABB IDrawable::GetWorldAABB() const
{
	// AABB transformation from 3D Math Primer for Graphics
	// and Game Development
	// Minimize each element of the transform matrix to get
	// the transformed AABB

	const FMatrix4& WorldTransform(GetWorldTransform());
	const AABB& Box(mBoundingBox);

	// Start with the object origin
	Vector3f Min = WorldTransform.GetOrigin();
	Vector3f Max(Min);

	// First column
	if (WorldTransform.M[0][0] > 0.0f)
	{
		Min.x += WorldTransform.M[0][0] * Box.Min.x; Max.x += WorldTransform.M[0][0] * Box.Max.x;
	}
	else
	{
		Min.x += WorldTransform.M[0][0] * Box.Max.x; Max.x += WorldTransform.M[0][0] * Box.Min.x;
	}

	if (WorldTransform.M[1][0] > 0.0f)
	{
		Min.y += WorldTransform.M[1][0] * Box.Min.x; Max.y += WorldTransform.M[1][0] * Box.Max.x;
	}
	else
	{
		Min.y += WorldTransform.M[1][0] * Box.Max.x; Max.y += WorldTransform.M[1][0] * Box.Min.x;
	}

	if (WorldTransform.M[2][0] > 0.0f)
	{
		Min.z += WorldTransform.M[2][0] * Box.Min.x; Max.z += WorldTransform.M[2][0] * Box.Max.x;
	}
	else
	{
		Min.z += WorldTransform.M[2][0] * Box.Max.x; Max.z += WorldTransform.M[2][0] * Box.Min.x;
	}

	// Second column
	if (WorldTransform.M[0][1] > 0.0f)
	{
		Min.x += WorldTransform.M[0][1] * Box.Min.y; Max.x += WorldTransform.M[0][1] * Box.Max.y;
	}
	else
	{
		Min.x += WorldTransform.M[0][1] * Box.Max.y; Max.x += WorldTransform.M[0][1] * Box.Min.y;
	}

	if (WorldTransform.M[1][1] > 0.0f)
	{
		Min.y += WorldTransform.M[1][1] * Box.Min.y; Max.y += WorldTransform.M[1][1] * Box.Max.y;
	}
	else
	{
		Min.y += WorldTransform.M[1][1] * Box.Max.y; Max.y += WorldTransform.M[1][1] * Box.Min.y;
	}

	if (WorldTransform.M[2][1] > 0.0f)
	{
		Min.z += WorldTransform.M[2][1] * Box.Min.y; Max.z += WorldTransform.M[2][1] * Box.Max.y;
	}
	else
	{
		Min.z += WorldTransform.M[2][1] * Box.Max.y; Max.z += WorldTransform.M[2][1] * Box.Min.y;
	}

	// Third column
	if (WorldTransform.M[0][2] > 0.0f)
	{
		Min.x += WorldTransform.M[0][2] * Box.Min.z; Max.x += WorldTransform.M[0][2] * Box.Max.z;
	}
	else
	{
		Min.x += WorldTransform.M[0][2] * Box.Max.z; Max.x += WorldTransform.M[0][2] * Box.Min.z;
	}

	if (WorldTransform.M[1][2] > 0.0f)
	{
		Min.y += WorldTransform.M[1][2] * Box.Min.z; Max.y += WorldTransform.M[1][2] * Box.Max.z;
	}
	else
	{
		Min.y += WorldTransform.M[1][2] * Box.Max.z; Max.y += WorldTransform.M[1][2] * Box.Min.z;
	}

	if (WorldTransform.M[2][2] > 0.0f)
	{
		Min.z += WorldTransform.M[2][2] * Box.Min.z; Max.z += WorldTransform.M[2][2] * Box.Max.z;
	}
	else
	{
		Min.z += WorldTransform.M[2][2] * Box.Max.z; Max.z += WorldTransform.M[2][2] * Box.Min.z;
	}

	return AABB(Min, Max);
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