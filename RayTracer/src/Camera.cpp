#include "Camera.h"

Camera::Camera(Vector3f EyePosition, Vector3f LookDirection, float FOV, Vector2i OutputResolution)
	: mPosition(EyePosition)
	, mDirection(LookDirection)
	, mFieldOfView(FOV)
	, mOutputResolution(OutputResolution)
	, mDistanceFromScreenPlane((1 / std::tan(((FOV / 180.0f) * 3.14159265f) / 2.0f)))
	, mAspectRatio((float)OutputResolution.y / OutputResolution.x)
{

}

Ray Camera::generateRay(int32_t X, int32_t Y) const
{
	// Calculate coordinates of pixel on screen plane (u, v, d)
	const float u = -1 + (2 * (X + 0.5f)) / mOutputResolution.x;
	const float v = mAspectRatio - (2 * mAspectRatio * (Y + 0.5f)) / mOutputResolution.y;

	// Compute direction of ray
	Vector3f rayDirection(u, v, -mDistanceFromScreenPlane);
	rayDirection.Normalize();

	return Ray(mPosition, rayDirection);
}

float Camera::getFOV() const
{
	return mFieldOfView;
}

void Camera::setFOV(float FOV)
{
	mFieldOfView = FOV;
	mDistanceFromScreenPlane = (float)(1 / std::tan((FOV / 180 * 3.14159265) / 2));
}

Vector3f Camera::getDirection() const
{
	return mDirection;
}

void Camera::setDirection(Vector3f direction)
{
	mDirection = direction;
	mDirection.Normalize();
}

Vector3f Camera::getPosition() const
{
	return mPosition;
}

void Camera::setPosition(Vector3f position)
{
	mPosition = position;
}