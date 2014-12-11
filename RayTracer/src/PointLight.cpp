#include "PointLight.h"
#include "Matrix4.h"

FPointLight::FPointLight(FColor LightColor, Vector3f LightPosition, float SizeRadius, float MinDistance, float MaxDistance)
	: ILight(LightColor)
	, mPosition(LightPosition)
	, mSizeRadius(SizeRadius)
	, mMinRange(MinDistance)
	, mMaxRange(MaxDistance)
{

}

FRay FPointLight::GetRayToLight(const Vector3f& SurfacePoint) const
{
	return FRay(SurfacePoint, mPosition - SurfacePoint);
}

std::vector<FRay> FPointLight::GetRayToLightSamples(const Vector3f& SurfacePoint, int NumSamples) const
{
	// create a plane, decompose it into grids, then take samples
	const Vector3f SurfaceDirection((mPosition - SurfacePoint).Normalize());
	 
	//  let the up direction be the smallest component of the surface direction
	int ShortestDirection = 0;
	for (int i = 1; i < 3; i++)
		if (SurfaceDirection[i] < SurfaceDirection[ShortestDirection])
			ShortestDirection = i;

	// calculate basis vectors for the plan
	Vector3f vUp;
	vUp[ShortestDirection] = 1.0f;

	const Vector3f RightVector(Vector3f::Cross(SurfaceDirection, vUp));
	vUp = Vector3f::Cross(RightVector, SurfaceDirection);

	// create a new frame from these vectors
	FMatrix4 LightPlaneFrame(RightVector, vUp, SurfaceDirection);
	LightPlaneFrame.SetOrigin(mPosition);

	const float SamplePlaneRadius = mSizeRadius;
	const int NumRows = (int)sqrt(NumSamples);
	const float GridSize = mSizeRadius / NumRows;

	// generate rays in world space
	std::vector<FRay> SampleRays;

	for (int i = 0; i < NumRows; i++)
	{
		const float Z = i * GridSize - SamplePlaneRadius;
		for (int j = 0; j < NumRows; j++)
		{
			const float X = j * GridSize - SamplePlaneRadius;
			Vector3f GridPosition(X, 0, Z);
			GridPosition = LightPlaneFrame.TransformPosition(GridPosition);

			SampleRays.push_back(FRay(SurfacePoint, GridPosition - SurfacePoint));
		}
	}

	SampleRays.resize(NumSamples);
	return SampleRays;
}

void FPointLight::SetLightPosition(const Vector3f& LightPosition)
{
	mPosition = LightPosition;
}

Vector3f FPointLight::GetLightPosition() const
{
	return mPosition;
}

FColor FPointLight::GetIntesityAt(Vector3f Position) const
{
	float Distance = (Position - mPosition).Length();
	if (Distance < mMinRange)
		return mLightColor;
	else if (Distance < mMaxRange)
		return mLightColor * ((mMaxRange - Distance) / (mMaxRange - mMinRange));
	else
		return FColor::Black;
}

FPointLight::~FPointLight()
{
}
