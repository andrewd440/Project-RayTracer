#pragma once

#include "Camera.h"
#include "Image.h"
#include "KDTree.h"
#include "Light.h"
#include "Drawable.h"

#include <vector>
#include <memory>
#include <istream>

/** 
* Represents a scene that will be raytraced 
* and rendered to an image. 
*/
class FScene
{
public:
	using PrimitivePtr = std::unique_ptr<IDrawable>;
	using LightPtr = std::unique_ptr<ILight>;

	/**
	* Default constructor.
	*/
	FScene();

	// Don't allow copies of a scene
	FScene& operator=(const FScene& Copy) = delete;

	/**
	* Builds the scene. 
	* Add the camera, lights, and geometry to the scene.
	* @param SceneConfig - Scene setup file stream
	*/
	void BuildScene(std::istream& SceneConfig);

	/**
	* Traces a ray into the scene and computes the resulting color
	* from the source point.
	* @param CameraRay - A ray generated from the viewpoint through a pixel
	*						on the screen.
	* @return The resulting color for the source pixel.
	*/
	FColor TraceRay(const FRay& CameraRay, int32_t Depth);

	/**
	* Renders the scene to an image.
	*/
	void RenderScene();

private:
	/**
	* Computes a specular reflection based on the Blinn Model for Specular Reflection.
	* @param LightDirection - The Normalized direction of the light
	* @param ViewerDirection - The Normalized direction of the viewer
	* @return Normalized direction vector for the specular reflection 
	*/
	Vector3f ComputeBlinnSpecularReflection(const Vector3f& LightDirection, const Vector3f& ViewerDirection) const;

	/**
	* Computes the refraction vector of a surface based on Snell's law.
	* @param LightDirection of incoming light.
	* @param SurfaceNormal of the refractive surface.
	* @param RefractiveIndex of the surface.
	* @param Normalized direction of the refraction vector.
	*/
	Vector3f ComputeRefractionVector(const Vector3f& LightDirection, const Vector3f& SurfaceNormal, const float& RefractiveIndex) const;

	/**
	* Checks if a light ray is blocked by another object.
	* @param LightRay - A ray from the surface point on the Primitive to the light source
	* @return True if the point is in a shadow
	*
	*/
	bool IsInShadow(const FRay& LightRay);

	/**
	* Computes the factor of a light that is visible to a surface point.
	* @param Light to check against
	* @param SurfacePoint to test
	* @return Value between 0-1 for the factor of light that is visible to the surface 
	*
	*/
	float ComputeShadeFactor(const ILight& Light, const Vector3f& SurfacePoint) const;

	/**
	* Computes a mirror reflection from a light direction and a reflection normal
	* @param ViewerDirection - Direction of the viewer
	* @param SurfaceNormal - Normal to be reflected across
	* @return A direction vector for the mirror reflection
	*/
	Vector3f ComputeMirriorReflection(const Vector3f& ViewerDirection, const Vector3f& SurfaceNormal) const;

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
	FMaterial ReadMaterial(std::istream& Input);

	/**
	* Reads in a model from a file of vertices and faces. Applies a translation
	* if given;
	* @param Filename - File path for the model.
	* @param Translation - 3D translation values.
	* @param MaterialProperties - Model lighting material.
	*/
	void ReadModel(std::string Filename, Vector3f Translation, FMaterial MaterialProperties);

private:
	FImage mOutputImage; /* Output image for the rendered scene */
	FCamera mCamera; /* FCamera for the scene */
	std::vector<PrimitivePtr> mPrimitives; /* All Primitives in the scene */
	std::vector<LightPtr> mLights; /* All lights in the scene */
	FColor mBackgroundColor; /* Background color for the scene */
	FColor mGlobalAmbient; /* Color used for global ambient lighting */
	KDTree mKDTree;

	uint16_t mNumberOfShadowSamples; /* Number of samples to use when generating shadows */
	uint8_t mSuperSamplingLevel; /* The number of rays generated per pixel is squared this number */
};