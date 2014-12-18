#pragma once
#include "Vector3.h"

#define _PI 3.14159265f
#define _EPSILON 0.00001f

enum class EAxis
{
	X,
	Y,
	Z
};

/**
* Checks the bounds of a vector against current min and max bounds.
* The min and max bounds are updated if the new point is outside the current
* bounds.
* @param CurrentMin The current min bounds.
* @param CurrentMax The current max bounds.
* @param NewPoint The point to check.
*/
inline void UpdateBounds(Vector3f& CurrentMin, Vector3f& CurrentMax, const Vector3f& NewPoint)
{
	for (int i = 0; i < 3; i++)
	{
		if (CurrentMax[i] < NewPoint[i])
		{
			CurrentMax[i] = NewPoint[i];
		}
		else if (CurrentMin[i] > NewPoint[i])
		{
			CurrentMin[i] = NewPoint[i];
		}
	}
}

inline void ComputeBarycentric(	const Vector3f& V0, 
								const Vector3f& V1, 
								const Vector3f& V2, 
								const Vector3f& SurfacePoint,
								float BOut[3])
{
	// compute counter clockwise edge vectors
	const Vector3f& e1 = V1 - V2;
	const Vector3f& e2 = V2 - V0;
	const Vector3f& e3 = V0 - V1;

	const Vector3f& Normal = Vector3f::Cross(e2, e1);

	// compute vectors from point to each vertex
	const Vector3f& d1 = SurfacePoint - V0;
	const Vector3f& d2 = SurfacePoint - V1;
	const Vector3f& d3 = SurfacePoint - V2;

	// compute denominator for each barycentric coordinate calculation
	const float& Denom = 1.0f / Vector3f::Dot(Vector3f::Cross(e2, e1), Normal);

	BOut[0] = Vector3f::Dot(Vector3f::Cross(e1, d3), Normal) * Denom;
	BOut[1] = Vector3f::Dot(Vector3f::Cross(e2, d1), Normal) * Denom;
	BOut[2] = Vector3f::Dot(Vector3f::Cross(e3, d2), Normal) * Denom;
}