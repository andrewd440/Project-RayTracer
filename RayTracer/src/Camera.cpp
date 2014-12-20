#include "Camera.h"

FCamera::FCamera(const Vector3f& EyePosition, const Vector3f& LookAt, const Vector3f& UpDirection, float FOV, const Vector2i& OutputResolution)
	: mFieldOfView(FOV)
	, mDistanceFromScreenPlane((1 / std::tan(((FOV / 180.0f) * 3.14159265f) / 2.0f)))
	, mAspectRatio((float)OutputResolution.y / OutputResolution.x)
	, mOutputResolution(OutputResolution)
	, mViewTransform(EyePosition, LookAt, UpDirection)
{
	// since we will be transform our pixel rays into world space,
	// get the inverse of the normal LookAt matrix
	mViewTransform = mViewTransform.GetInverseAffine();
}

FRay FCamera::GenerateRay(int32_t X, int32_t Y) const
{
	// Calculate coordinates of pixel on screen plane (u, v, d)
	const float U = -1 + (2 * (X + 0.5f)) / mOutputResolution.x;
	const float V = mAspectRatio - (2 * mAspectRatio * (Y + 0.5f)) / mOutputResolution.y;

	// Compute direction of ray in world space
	Vector3f rayDirection = Vector3f(-U, V, -mDistanceFromScreenPlane);
	rayDirection.Normalize();

	const FRay PixelRay(Vector3f(), rayDirection);

	// send ray in world space
	return mViewTransform.TransformRay(PixelRay);
}

std::vector<FRay> FCamera::GenerateSampleRays(int32_t X, int32_t Y, uint16_t SamplingLevel) const
{
	// adjust output resolution according to division of current pixels and
	// store the inverse of this for use in UV calculations
	const float InvOutputResX = 1.0f / (mOutputResolution.x * SamplingLevel);
	const float InvOutputResY = 1.0f / (mOutputResolution.y * SamplingLevel);
	const float InvHundred = 1.0f / 100.0f; // store for random UV offsets

	// we are simulating more pixels with the UV, so adjust our current pixels
	X *= SamplingLevel; Y *= SamplingLevel;

	std::vector<FRay> SampleRays;
	for (int i = 0; i < SamplingLevel; i++)
	{
		for (int j = 0; j < SamplingLevel; j++)
		{
			// Get random U and V offsets within the pixel
			const float UOffset = (std::rand() % 100 + 1) * InvHundred;
			const float VOffset = (std::rand() % 100 + 1) * InvHundred;

			// Calculate coordinates of pixel on screen plane (u, v, d)
			const float U = -1 + (2 * (X + UOffset)) * InvOutputResX;
			const float V = mAspectRatio - (2 * mAspectRatio * (Y + VOffset)) * InvOutputResY;

			// Compute direction of ray in world space
			Vector3f RayDirection = Vector3f(-U, V, -mDistanceFromScreenPlane);

			// Take ray into world space
			FRay PixelRay(Vector3f(), RayDirection.Normalize());
			PixelRay = mViewTransform.TransformRay(PixelRay);

			SampleRays.push_back(PixelRay);
			X++;
		}
		Y++;
		X -= SamplingLevel; // reset X for next row
	}

	return SampleRays;
}

float FCamera::GetFOV() const
{
	return mFieldOfView;
}

void FCamera::SetFOV(float FOV)
{
	mFieldOfView = FOV;
	mDistanceFromScreenPlane = (float)(1 / std::tan((FOV / 180 * 3.14159265) / 2));
}