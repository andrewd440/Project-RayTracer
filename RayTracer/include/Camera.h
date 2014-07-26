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
	* Constructs a scene camera from it's world position, direction, distance from the screen, and 
	* screen size.
	* @param camerPosition - world position of the camera
	* @param cameraDirection - normalized direction vector of the camera's direction
	* @param distanceFromScreen - the distance of the camera from the screen
	* @param screenSize - size of the screen (in pixels)
	*/
	Camera(Vector3f cameraPosition, Vector3f cameraDirection, float distanceFromScreen, Vector2i screenSize);

	/**
	* Generates a ray from the viewpoint through a screen pixel.
	* @param x - horizontal pixel position
	* @param y - vertical pixel position
	* @return The generated ray
	*/
	Ray generateRay(float x, float y) const;

	/**
	* Retrieve the size (in pixels) of the screen.
	* @return Size of screen
	*/
	Vector2i getScreenSize() const;

	/**
	* Sets the size of the screen.
	* @param screenSize - size of the screen
	*/
	void setScreenSize(Vector2i screenSize);

	/**
	* Retrieves the distance of the viewpoint from the screen
	* @returns The distance between the camera and screen
	*/
	float getDistanceFromScreen() const;

	/**
	* Sets the distance between the viewpoint and the screen.
	* @param distance - distance between camera and screen
	*/
	void setDistanceFromScreen(float distance);

private:
	float mDistanceFromScreen; /* Distance between viewpoint and screen */
	Vector2i mScreenSize; /* Size, in pixels, of the screen */
	Vector3f mDirection; /* Normalized direction of the camera */
	Vector3f mPosition; /* World position of the camera */
};

