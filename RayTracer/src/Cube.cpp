#include "Cube.h"
#include "Intersection.h"

#include <limits>


FCube::FCube(Vector3f Center, FMaterial LightingMaterial)
	: IDrawable(LightingMaterial)
{
	SetOrigin(Center);
	ConstructAABB();
}

bool FCube::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	// just use bounding box as intersection test
	const float OriginalT = (tValueOut) ? *tValueOut : 0;

	// compute intersection in object space
	Ray = GetWorldInvTransform().TransformRay(Ray);

	const bool IsIntersecting = GetBoundingBox().IsIntersectingRay(Ray, tValueOut, IntersectionOut);

	// Ray intersects all slabs, return nearest t value if less than tValueOut
	if (IsIntersecting && tValueOut && OriginalT > *tValueOut)
	{
		// get intersection point in world space
		Vector3f IntersectionPoint = GetWorldTransform().TransformPosition(Ray.origin + *tValueOut * Ray.direction);
		ConstructIntersection(IntersectionPoint, IntersectionOut);
	}

	return IsIntersecting;
}

void FCube::ConstructIntersection(const Vector3f& IntersectionPoint, FIntersection* IntersectionOut)
{
	float LargestSide = 0;
	int IntersectionAxis = 0;
	int IntersectionSide = 0;

	const FMatrix4& ModelInverse = GetWorldInvTransform();

	const Vector3f IntersectionInObjectSpace = ModelInverse.TransformPosition(IntersectionPoint);

	// get the intersection plane
	for (int i = 0; i < 3; i++)
	{
		// get axis with largest length
		if (abs(IntersectionInObjectSpace[i]) > LargestSide)
		{
			LargestSide = abs(IntersectionInObjectSpace[i]);
			IntersectionSide = (IntersectionInObjectSpace[i] < 0.0f) ? -1 : 1;
			IntersectionAxis = i;
		}
	}

	Vector3f Normal;
	Normal[IntersectionAxis] = (float)IntersectionSide;
	Normal = ModelInverse.TransformDirection(Normal).Normalize();

	IntersectionOut->normal = Normal;
	IntersectionOut->object = this;

	// make sure point not inside of the object
	IntersectionOut->point = IntersectionPoint + (Normal * _EPSILON);
}

void FCube::ConstructAABB(Vector3f Min, Vector3f Max)
{
	Min = Vector3f(-1, -1, -1);
	Max = Vector3f(1, 1, 1);

	SetBoundingBox(AABB{ Min, Max });
}

FMaterial FCube::GetMaterial(Vector3f SurfacePoint)
{
	const FTextureInfo& DiffuseInfo = mMaterial.GetDiffuseTexture();
	if (!DiffuseInfo.Texture)
		return mMaterial;

	SurfacePoint = GetWorldInvTransform().TransformPosition(SurfacePoint);
	const Vector3f& Scale = GetWorldTransform().GetScale();

	float U = 0.0f, V = 0.0f;
	if (1.0f - abs(SurfacePoint.x) < _EPSILON)
	{
		U = Scale.z * (SurfacePoint.z + 1.0f) / 2.0f;
		V = Scale.y * (SurfacePoint.y + 1.0f) / 2.0f;
	}
	else if (1.0f - abs(SurfacePoint.y)< _EPSILON)
	{
		U = Scale.x * (SurfacePoint.x + 1.0f) / 2.0f;
		V = Scale.z * (SurfacePoint.z + 1.0f) / 2.0f;
	}
	else
	{
		U = Scale.x * (SurfacePoint.x + 1.0f) / 2.0f;
		V = Scale.y * (SurfacePoint.y + 1.0f) / 2.0f;
	}

	U *= DiffuseInfo.UAxisScale;
	V *= DiffuseInfo.VAxisScale;

	mMaterial.SetDiffuse(DiffuseInfo.Texture->GetSample(U, V));

	return mMaterial;
}

FCube::~FCube()
{
}
