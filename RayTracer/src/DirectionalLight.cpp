#include "DirectionalLight.h"
#include "Matrix4.h"

#include <cstdlib>
#include <limits>

FDirectionalLight::FDirectionalLight()
	: ILight()
	, mLightDirection(0,-1,0)
{
}

FDirectionalLight::FDirectionalLight(FColor LightColor, Vector3f LightDirection)
	: ILight(LightColor)
	, mLightDirection(LightDirection)
{
	mLightDirection.Normalize();
}


FRay FDirectionalLight::GetRayToLight(const Vector3f& SurfacePoint) const
{
	return FRay(SurfacePoint, -mLightDirection);
}

std::vector<FRay> FDirectionalLight::GetRayToLightSamples(const Vector3f& SurfacePoint, int NumSamples) const
{
	// make a new frame that points in the direction of the light
	const Vector3f N = mLightDirection;

	//  let the up direction be the smallest component of the light direction
	int ShortestDirection = 0;
	for (int i = 1; i < 3; i++)
		if (abs(N[i]) < abs(N[ShortestDirection]))
			ShortestDirection = i;

	// calculate basis vectors for the frame
	Vector3f vUp;
	vUp[ShortestDirection] = 1.0f;

	const Vector3f RightVector(Vector3f::Cross(N, vUp).Normalize());
	vUp = Vector3f::Cross(RightVector, N).Normalize();

	const FMatrix4 LightFrame(RightVector, vUp, N);
	std::vector<FRay> RaySamples;

	// randomly move the direction vector small amounts for each sample
	//const float MaxMovement = 0.1f;
	for (int i = 0; i < NumSamples; i++)
	{
		const float XMovement = std::rand() % 100 / 1000.0f;
		const float ZMovement = std::rand() % 100 / 1000.0f;
		Vector3f SampleDirectionOffset(XMovement, 0.0f, ZMovement);
		SampleDirectionOffset = LightFrame.TransformPosition(SampleDirectionOffset);

		// turn the ray to the direction of the light and add the normal
		// to the ray origin so we are sure that we are not under the surface
		FRay SampleRay(SurfacePoint, (-mLightDirection + SampleDirectionOffset).Normalize());
		SampleRay.origin += SampleRay.direction * _EPSILON;
		RaySamples.push_back(SampleRay);
	}

	return RaySamples;
}

void FDirectionalLight::setLightDirection(const Vector3f& LightDirection)
{
	mLightDirection = LightDirection;
	mLightDirection.Normalize();
}

Vector3f FDirectionalLight::getLightDirection() const
{
	return mLightDirection;
}

float FDirectionalLight::GetDistance(const Vector3f Position) const
{
	// disable compiler warning
	Position;

	return std::numeric_limits<float>::max();
}

FColor FDirectionalLight::GetIntesityAt(Vector3f Position) const
{
	Position; // turn off compiler warning
	return mLightColor;
}