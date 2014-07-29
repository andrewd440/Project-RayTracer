#pragma once
#include "Light.h"
#include "Sphere.h"
#include "Camera.h"
#include "Image.h"

#include <vector>
#include <memory>

/** 
* Represents a scene that will be raytraced 
* and rendered to an image. 
*/
class Scene
{
public:
	typedef std::unique_ptr<Shape> ShapePtr;
	typedef std::unique_ptr<Light> LightPtr;

	/**
	* Default constructor.
	*/
	Scene();

	/**
	* Builds the scene. 
	* Add the camera, lights, and geometry to the scene.
	*/
	void buildScene();

	/**
	* Traces a ray into the scene and computes the resulting color
	* from the source point.
	* @param CameraRay - A ray generated from the viewpoint through a pixel
	*						on the screen.
	* @return The resulting color for the source pixel.
	*/
	Color traceRay(const Ray& CameraRay);

	/**
	* Renders the scene to an image.
	*/
	void renderScene();



private:
	Image mOutputImage; /* Output image for the rendered scene */
	Camera mCamera; /* Camera for the scene */
	std::vector<ShapePtr> mShapes; /* All shapes in the scene */
	std::vector<LightPtr> mLights; /* All lights in the scene */
};

