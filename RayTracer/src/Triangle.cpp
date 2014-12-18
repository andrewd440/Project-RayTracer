#include "Triangle.h"
#include "Intersection.h"
#include "Texture.h"

#include <cmath>

FTriangle::FTriangle()
	: IDrawable()
	, mV0()
	, mV1()
	, mV2()
	, mNormal()
	, mUV0()
	, mUV1()
	, mUV2()
{

}

FTriangle::FTriangle(Vector3f V0, Vector3f V1, Vector3f V2, const FMaterial& LightingMaterial)
	: IDrawable(LightingMaterial)
	, mV0(V0)
	, mV1(V1)
	, mV2(V2)
	, mUV0()
	, mUV1()
	, mUV2()
{
	Vector3f e1 = V2 - V0;
	Vector3f e2 = V1 - V0;
	mNormal = Vector3f::Cross(e1, e2);
	mNormal.Normalize();
	ConstructAABB();
}

FTriangle::FTriangle(Vector3f V0, Vector3f V1, Vector3f V2, Vector3f Normal, const FMaterial& LightingMaterial)
	: IDrawable(LightingMaterial)
	, mV0(V0)
	, mV1(V1)
	, mV2(V2)
	, mNormal(Normal)
	, mUV0()
	, mUV1()
	, mUV2()
{

}

bool FTriangle::IsIntersectingRay(FRay ray, float* tValueOut, FIntersection* intersectionOut)
{
	// Ray/Triangle intersection test from 3D Math Primier for Graphics and Game Development

	// Compute gradient, how steep is the ray against the triangle
	float gradient = Vector3f::Dot(mNormal, ray.direction);

	// Check if ray is pointing towards the triangle
	if (!(gradient < 0.0f))
		return false;

	// Compute value for plane equation, Ax + Bx + Cz = d
	float d = Vector3f::Dot(mNormal, mV0);

	// Compute parametric point of intersection with plane.
	float t = d - Vector3f::Dot(mNormal, ray.origin);

	// Bail if ray is on backside of plane
	if (!(t <= 0.0f))
		return false;

	// Check if tValueOut is closer
	if (tValueOut && !(t >= gradient * *tValueOut))
		return false;

	// The ray intersects the plane, now find the point
	t /= gradient;
	assert(t >= 0.0f);
	Vector3f p = ray.origin + ray.direction * t;

	// Find dominant axis to select which plane to 
	// project onto, and compute u's and v's
	float u0, u1, u2;
	float v0, v1, v2;

	if (fabs(mNormal.x) > fabs(mNormal.y))
	{
		if (fabs(mNormal.x) > fabs(mNormal.z))
		{
			u0 = p.y - mV0.y;
			u1 = mV1.y - mV0.y;
			u2 = mV2.y - mV0.y;

			v0 = p.z - mV0.z;
			v1 = mV1.z - mV0.z;
			v2 = mV2.z - mV0.z;
		}
		else
		{
			u0 = p.x - mV0.x;
			u1 = mV1.x - mV0.x;
			u2 = mV2.x - mV0.x;

			v0 = p.y - mV0.y;
			v1 = mV1.y - mV0.y;
			v2 = mV2.y - mV0.y;
		}
	}
	else
	{
		if (fabs(mNormal.y) > fabs(mNormal.z))
		{
			u0 = p.x - mV0.x;
			u1 = mV1.x - mV0.x;
			u2 = mV2.x - mV0.x;

			v0 = p.z - mV0.z;
			v1 = mV1.z - mV0.z;
			v2 = mV2.z - mV0.z;
		}
		else
		{
			u0 = p.x - mV0.x;
			u1 = mV1.x - mV0.x;
			u2 = mV2.x - mV0.x;

			v0 = p.y - mV0.y;
			v1 = mV1.y - mV0.y;
			v2 = mV2.y - mV0.y;
		}
	}

	// Compute denominator
	float temp = u1 * v2 - v1 * u2;
	if (!(temp != 0.0f))
		return false;
	temp = 1.f / temp;

	// Compute barycentric coords, exit on out-of-range
	float alpha = (u0 * v2 - v0 * u2) * temp;
	if (!(alpha >= 0.0f))
		return false;

	float beta = (u1 * v0 - v1 * u0) * temp;
	if (!(beta >= 0.0f))
		return false;

	float gamma = 1.f - alpha - beta;
	if (!(gamma >= 0.0f))
		return false;

	if (tValueOut && t < *tValueOut)
	{
		*tValueOut = t;
		ConstructIntersection(ray.origin + t * ray.direction, intersectionOut);
	}

	return true;
}

FMaterial FTriangle::GetMaterial(Vector3f SurfacePoint)
{
	// bring the point into object space
	SurfacePoint = GetWorldInvTransform().TransformPosition(SurfacePoint);
	const FTextureInfo& DiffuseInfo = mMaterial.GetDiffuseTexture();

	// if no texture, return default material
	if (!DiffuseInfo.Texture)
		return mMaterial;

	float B[3];
	ComputeBarycentric(mV0, mV1, mV2, SurfacePoint, B);

	Vector2f UV = B[0] * mUV0 + B[1] * mUV1 + B[2] * mUV2;
	UV.x *= DiffuseInfo.UAxisScale;
	UV.y *= DiffuseInfo.VAxisScale;

	mMaterial.SetDiffuse(DiffuseInfo.Texture->GetSample(UV.x, UV.y));
	
	return mMaterial;
}

void FTriangle::SetUVCoordinates(const Vector2f& UV0, const Vector2f& UV1, const Vector2f& UV2)
{
	mUV0 = UV0;
	mUV1 = UV1;
	mUV2 = UV2;
}

void FTriangle::ConstructIntersection(Vector3f intersectionPoint, FIntersection* intersectionOut)
{
	intersectionOut->object = this;
	intersectionOut->point = intersectionPoint + intersectionOut->normal * _EPSILON;
	intersectionOut->normal = mNormal;
}

void FTriangle::ConstructAABB(Vector3f Min, Vector3f Max)
{
	Max = Vector3f(std::max({ mV0.x, mV1.x, mV2.x }), std::max({ mV0.y, mV1.y, mV2.y }), std::max({ mV0.z, mV1.z, mV2.z }));
	Min = Vector3f(std::min({ mV0.x, mV1.x, mV2.x }), std::min({ mV0.y, mV1.y, mV2.y }), std::min({ mV0.z, mV1.z, mV2.z }));

	SetBoundingBox(AABB(Min, Max));
}