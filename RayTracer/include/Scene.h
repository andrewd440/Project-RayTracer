#pragma once

#include "Camera.h"
#include "Image.h"
#include "KDTree.h"
#include "Light.h"
#include "Primitive.h"

#include <vector>
#include <memory>
#include <istream>

/** 
* Represents a scene that will be raytraced 
* and rendered to an image. 
*/
class Scene
{
public:
	using PrimitivePtr =  std::unique_ptr<Primitive>;
	using LightPtr = std::unique_ptr<Light>;

	/**
	* Default constructor.
	*/
	Scene();

	// Don't allow copies of a scene
	Scene& operator=(const Scene& copy) = delete;

	/**
	* Builds the scene. 
	* Add the camera, lights, and geometry to the scene.
	* @param SceneConfig - Scene setup file stream
	*/
	void buildScene(std::istream& SceneConfig);

	/**
	* Traces a ray into the scene and computes the resulting color
	* from the source point.
	* @param CameraRay - A ray generated from the viewpoint through a pixel
	*						on the screen.
	* @return The resulting color for the source pixel.
	*/
	Color traceRay(const Ray& CameraRay, int32_t depth);

	/**
	* Renders the scene to an image.
	*/
	void renderScene();

private:
	/**
	* Computes a specular reflection based on the Blinn Model for Specular Reflection.
	* @param LightDirection - The Normalized direction of the light
	* @param ViewerDirection - The Normalized direction of the viewer
	* @return Normalized direction vector for the specular reflection 
	*/
	Vector3f computeBlinnSpecularReflection(const Vector3f& LightDirection, const Vector3f& ViewerDirection);

	/**
	* Checks if a light ray is blocked by another object.
	* @param LightRay - A ray from the surface point on the Primitive to the light source
	* @return True if the point is in a shadow
	*
	*/
	bool isInShadow(const Ray& LightRay);

	/**
	* Computes a mirror reflection from a light direction and a reflection normal
	* @param ViewerDirection - Direction of the viewer
	* @param SurfaceNormal - Normal to be reflected across
	* @return A direction vector for the mirror reflection
	*/
	Vector3f computeMirriorReflection(const Vector3f& ViewerDirection, const Vector3f& SurfaceNormal) const;

	/**
	* Reads in properties and creates a Material from an input stream 
	* @param Input - A input stream formated in the order of:
	*					Specular: - - -
	*					Diffuse: - - -
	*					Amient:  - - -
	*					SpecualarExponent: -
	*					Reflectivity: -
	* @return A Material with the given properties.
	*/
	Material readMaterial(std::istream& Input);

	/**
	* Reads in a model from a file of vertices and faces. Applies a translation
	* if given;
	* @param Filename - File path for the model.
	* @param Translation - 3D translation values.
	* @param MaterialProperties - Model lighting material.
	*/
	void readModel(std::string Filename, Vector3f Translation, Material MaterialProperties);

private:
	Image mOutputImage; /* Output image for the rendered scene */
	Camera mCamera; /* Camera for the scene */
	std::vector<PrimitivePtr> mPrimitives; /* All Primitives in the scene */
	std::vector<LightPtr> mLights; /* All lights in the scene */
	Color mBackgroundColor; /* Background color for the scene */
	Color mGlobalAmbient; /* Color used for global ambient lighting */
	KDTree mKDTree;
};