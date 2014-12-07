#include "Camera.h"

Camera::Camera(const Vector3f& EyePosition, const Vector3f& LookAt, const Vector3f& UpDirection, float FOV, const Vector2i& OutputResolution)
	: mFieldOfView(FOV)
	, mDistanceFromScreenPlane((1 / std::tan(((FOV / 180.0f) * 3.14159265f) / 2.0f)))
	, mAspectRatio((float)OutputResolution.y / OutputResolution.x)
	, mOutputResolution(OutputResolution)
{
	ViewTransform = LookAtMatrix(EyePosition, LookAt, UpDirection);
}

Ray Camera::GenerateRay(int32_t X, int32_t Y) const
{
	// Calculate coordinates of pixel on screen plane (u, v, d)
	const float u = -1 + (2 * (X + 0.5f)) / mOutputResolution.x;
	const float v = mAspectRatio - (2 * mAspectRatio * (Y + 0.5f)) / mOutputResolution.y;

	// Compute direction of ray in world space
	Vector3f rayDirection = Vector3f(u, v, -mDistanceFromScreenPlane);
	rayDirection.Normalize();

	return Ray(Vector3f(), rayDirection);
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
	ViewTransform.SetOrigin(Position);
}

LookAtMatrix Camera::ViewTransform(Vector3f(), Vector3f(0,0,-1), Vector3f(0, 1, 0));