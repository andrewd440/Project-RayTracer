#include "Camera.h"

Camera::Camera(Vector3f CameraPosition, Vector3f CameraDirection, float FOV, Vector2i OutputResolution)
	: mPosition(CameraPosition)
	, mDirection(CameraDirection)
	, mFieldOfView(FOV)
	, mOutputResolution(OutputResolution)
	, mDistanceFromScreenPlane((float)(1 / std::tan(((FOV / 180) * 3.14159265) / 2)))
	, mAspectRatio((float)OutputResolution.y / OutputResolution.x)
{

}

Ray Camera::generateRay(int X, int Y) const
{
	// Calculate coordinates of pixel on screen plane (u, v, d)
	const float u = -1 + (2 * (X + 0.5f)) / mOutputResolution.x;
	const float v = mAspectRatio - (2 * mAspectRatio * (Y + 0.5f)) / mOutputResolution.y;

	// Compute direction of ray
	Vector3f rayDirection(u, v, mDistanceFromScreenPlane);
	rayDirection.normalize();

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
	mDirection.normalize();
}

Vector3f Camera::getPosition() const
{
	return mPosition;
}

void Camera::setPosition(Vector3f position)
{
	mPosition = position;
}