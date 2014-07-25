#pragma once
#include "Vector3.h"
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
	Camera(Vector3f cameraPosition, Vector3f cameraDirection, float distanceFromScreen, Vector3i screenSize);

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
	Vector3i getScreenSize() const;

	/**
	* 
	*
	*/
	void setScreenSize(Vector3i screenSize);

	float getDistanceFromScreen() const;
	void setDistanceFromScreen(Vector3i distance);

private:
	float mDistanceFromScreen;
	Vector3i mScreenSize;
	Vector3f mDirection;
	Vector3f mPositon;
};

