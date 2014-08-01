#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Ray.h"

/**
* Represents a camera in the scene.
* Used to generate rays from a viewpoint through pixels on the screen.
*/
class Camera
{
public:
	/**
	* Default Constructor.
	* Sets up a camera with default values. Forward direction at origin with 
	* no FOV or output resolution.
	*/
	Camera();

	/**
	* Constructs a scene camera from it's world position, direction, distance from the screen, and 
	* screen size.
	* @param CameraPosition - world position of the camera
	* @param CameraDirection - normalized direction vector of the camera's direction
	* @param FOV - the distance of the camera from the screen
	* @param OutputResolution - size of the screen (in pixels)
	*/
	Camera(Vector3f CameraPosition, Vector3f CameraDirection, float FOV, Vector2i OutputResolution);

	/**
	* Generates a ray from the viewpoint through a screen pixel.
	* @param X - x coordinate of the pixel
	* @param Y - y coordinate of the pixel
	* @return The generated ray
	*/
	Ray generateRay(int X, int Y) const;

	/**
	* Retrieves the horizontal FOV of the camera.
	* @returns The horizontal field of view in degrees
	*/
	float getFOV() const;

	/**
	* Sets the horizontal FOV of the camera.
	* @param FOV - The horizontal field of view in degrees
	*/
	void setFOV(float FOV);

private:
	float mFieldOfView; /* Horizontal FOV of the camera */
	float mAspectRatio; /* Output resolution height/width */
	float mDistanceFromScreenPlane; /* Distance between viewpoint and screen */
	Vector2i mOutputResolution; /* Size, in pixels, of the output image */
	Vector3f mDirection; /* Normalized direction of the camera */
	Vector3f mPosition; /* World position of the camera */
};

