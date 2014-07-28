#pragma once
#include "Light.h"
#include "Shape.h"
#include "Camera.h"
#include "RayTracer.h"

#include <vector>

class Scene
{
public:
	Scene();
	~Scene();

	/**
	* Retrieves the scene camera.
	* @return The scene camera (unmodifiable)
	*/
	const Camera& getCamera() const;

	/**
	* Retrieves all shapes in the scene.
	* @return A const vector containing the shapes.
	*/
	const std::vector<Shape>& getShapes() const;

	/**
	* Retrieves all lights in the scene.
	* @return A const vector containing the lights.
	*/
	const std::vector<Light>& getLights() const;

private:
	RayTracer mRayTracer; /*  */
	Camera mCamera; /* Camera for the scene */
	std::vector<Shape> mShapes; /* All shapes in the scene */
	std::vector<Light> mLights; /* All lights in the scene */
};

