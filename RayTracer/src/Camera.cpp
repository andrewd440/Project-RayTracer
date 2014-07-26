#include "Camera.h"

Camera::Camera(Vector3f cameraPosition, Vector3f cameraDirection, float distanceFromScreen, Vector2i screenSize)
	: mPosition(cameraPosition)
	, mDirection(cameraDirection)
	, mDistanceFromScreen(distanceFromScreen)
	, mScreenSize(screenSize)
{

}

Ray Camera::generateRay(float x, float y) const
{
	const float halfWidth = mScreenSize.x / 2.f;
	const float halfHeight = mScreenSize.y / 2.f;

	// Calculate coordinates of pixel on screen plane (u, v, d)
	const float u = (mPosition.x - halfWidth) + (mScreenSize.x * (x + 0.5f)) / mScreenSize.x;
	const float v = (mPosition.y - halfHeight) + (mScreenSize.y * (y + 0.5f)) / mScreenSize.y;

	// Compute direction of ray
	Vector3f rayDirection(u, -v, mDistanceFromScreen);
	rayDirection.normalize();

	return Ray(mPosition, rayDirection);
}

Vector2i Camera::getScreenSize() const
{
	return mScreenSize;
}

void Camera::setScreenSize(Vector2i screenSize)
{
	mScreenSize = screenSize;
}

float Camera::getDistanceFromScreen() const
{
	return mDistanceFromScreen;
}

void Camera::setDistanceFromScreen(float distance)
{
	mDistanceFromScreen = distance;
}