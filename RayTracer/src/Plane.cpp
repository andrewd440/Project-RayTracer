#include "Plane.h"
#include "Intersection.h"
#include "Camera.h"
#include "Texture.h"

FPlane::FPlane(const FMaterial& LightingMaterial, Vector3f PlaneNormal, Vector3f PointOnPlane)
	: IDrawable(LightingMaterial)
	, mNormal(PlaneNormal.Normalize())
	, mDistanceFromOrigin()
	, mUAxis(mNormal.y, mNormal.z, -mNormal.x)
	, mVAxis(Vector3f::Cross(PlaneNormal, mUAxis).Normalize())
{
	mDistanceFromOrigin = Vector3f::Dot(-PlaneNormal, PointOnPlane);
	ConstructAABB();
}

bool FPlane::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	if (!IsEnabled())
		return false;

	// bring ray into object space
	Ray = GetWorldInvTransform().TransformRay(Ray);

	if (abs(Vector3f::Dot(Ray.direction, mNormal)) < _EPSILON)
		return false;

	// Plane/Ray intersection from Mathmatics for 3D Game Programming and Computer Graphics
	const float tValue = -(Vector3f::Dot(mNormal, Ray.origin) + mDistanceFromOrigin) / (Vector3f::Dot(mNormal, Ray.direction));

	if (tValue <= _EPSILON)
		return false;

	if (tValueOut)
	{
		if (IntersectionOut && tValue < *tValueOut)
		{
			*tValueOut = tValue;
			ConstructIntersection(Ray.origin + Ray.direction * tValue, *IntersectionOut);
		}
		else if (tValue > *tValueOut)
		{
			return false;
		}
	}

	return true;
}

FMaterial FPlane::GetMaterial(Vector3f SurfacePoint)
{
	const FTextureInfo& DiffuseTextureInfo = mMaterial.GetDiffuseTexture();

	if (!DiffuseTextureInfo.Texture)
		return mMaterial;

	// bring point into object space
	SurfacePoint = GetWorldInvTransform().TransformPosition(SurfacePoint);

	// calculate how much to point extends on each UAxis and VAxis
	const float& U = Vector3f::Dot(SurfacePoint, mUAxis) * DiffuseTextureInfo.UAxisScale;
	const float& V = Vector3f::Dot(SurfacePoint, mVAxis) * DiffuseTextureInfo.VAxisScale;

	mMaterial.SetDiffuse(DiffuseTextureInfo.Texture->GetSample(U, V));

	return mMaterial;
}

void FPlane::ConstructIntersection(Vector3f IntersectionPoint, FIntersection& IntersectionOut)
{
	IntersectionOut.object = this;
	IntersectionOut.normal = mNormal;
	IntersectionOut.point = GetWorldTransform().TransformPosition(IntersectionPoint + mNormal * _EPSILON);
}

void FPlane::ConstructAABB(Vector3f Min, Vector3f Max)
{
	Max.x = std::numeric_limits<float>::max();
	Min.x = -std::numeric_limits<float>::max();

	Max.y = std::numeric_limits<float>::max();;
	Min.y = -std::numeric_limits<float>::max();;

	Max.z = std::numeric_limits<float>::max();
	Min.z = -std::numeric_limits<float>::max();

	SetBoundingBox(AABB(Min, Max));
}