#pragma once

#include <cstdint>

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Ray.h"

/**
* Represents a camera in the scene.
* Used to generate rays from a viewpoint through pixels on the screen.
*/
class FCamera
{
public:
	/**
	* Constructs a scene camera from it's world position, direction, distance from the screen, and 
	* screen size.
	* @param EyePosition World position of the camera
	* @param LookAt Position the camera is facing
	* @param UpDirection Direction toward up.
	* @param FOV The distance of the camera from the screen
	* @param OutputResolution Size of the screen (in pixels)
	*/
	FCamera(const Vector3f& EyePosition, const Vector3f& LookAt, const Vector3f& UpDirection, float FOV, const Vector2i& OutputResolution);

	/**
	* Generates a ray from the viewpoint through a screen pixel.
	* @param X - x coordinate of the pixel
	* @param Y - y coordinate of the pixel
	* @return The generated ray in world coordinates
	*/
	FRay GenerateRay(int32_t X, int32_t Y) const;

	/**
	* Retrieves the horizontal FOV of the camera.
	* @returns The horizontal field of view in degrees
	*/
	float GetFOV() const;

	/**
	* Sets the horizontal FOV of the camera.
	* @param FOV - The horizontal field of view in degrees
	*/
	void SetFOV(float FOV);

private:
	float mFieldOfView; /* Horizontal FOV of the camera */
	float mAspectRatio; /* Output resolution height/width */
	float mDistanceFromScreenPlane; /* Distance between viewpoint and screen */
	Vector2i mOutputResolution; /* Size, in pixels, of the output image */
	LookAtMatrix mViewTransform; /* From View space to world space */
};

