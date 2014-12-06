#include "Camera.h"

Camera::Camera(const Vector3f& EyePosition, const Vector3f& LookDirection, const Vector3f& UpDirection, float FOV, const Vector2i& OutputResolution)
	: mViewTransform(EyePosition, LookDirection, UpDirection)
	, mFieldOfView(FOV)
	, mDistanceFromScreenPlane((1 / std::tan(((FOV / 180.0f) * 3.14159265f) / 2.0f)))
	, mAspectRatio((float)OutputResolution.y / OutputResolution.x)
	, mOutputResolution(OutputResolution)
{

}

Ray Camera::GenerateRay(int32_t X, int32_t Y) const
{
	// Calculate coordinates of pixel on screen plane (u, v, d)
	const float u = -1 + (2 * (X + 0.5f)) / mOutputResolution.x;
	const float v = mAspectRatio - (2 * mAspectRatio * (Y + 0.5f)) / mOutputResolution.y;

	// Compute direction of ray in world space
	Vector3f rayDirection = mViewTransform.TransformDirection(Vector3f(u, v, -mDistanceFromScreenPlane));
	rayDirection.Normalize();

	return Ray(mViewTransform.GetOrigin(), rayDirection);
}

float Camera::GetFOV() const
{
	return mFieldOfView;
}

void Camera::SetFOV(float FOV)
{
	mFieldOfView = FOV;
	mDistanceFromScreenPlane = (float)(1 / std::tan((FOV / 180 * 3.14159265) / 2));
}

void Camera::SetPosition(const Vector3f& Position)
{
	mViewTransform.SetOrigin(Position);
}

LookAtMatrix Camera::GetViewTransform() const
{
	return mViewTransform;
}