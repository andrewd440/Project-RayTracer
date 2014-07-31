#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "LocalGeometry.h"
#include "Vector3.inl"
#include "DirectionalLight.h"

#include <algorithm>

const Vector2i OUTPUT_RESOLUTION(800, 500);
const Color BACKGROUND_COLOR(0, 0, 0);

Scene::Scene()
	: mCamera(Vector3f(0, 0, 0), Vector3f(0,0,1), 65, OUTPUT_RESOLUTION)
	, mOutputImage("RenderedScene", OUTPUT_RESOLUTION)
	, mShapes()
	, mLights()
{
	buildScene();
}


void Scene::buildScene()
{
	Material redMaterial(Color(100,100,100), Color(210,40,50), Color::White, 64);
	ShapePtr shape(new Sphere(Vector3f(0, 0, 20), 4, redMaterial));
	mShapes.push_back(std::move(shape));

	Vector3f lightDirection(-3, -3, 5);
	lightDirection.normalize();
	LightPtr light(new DirectionalLight(Color::White, lightDirection));
	mLights.push_back(std::move(light));

	Vector3f lightDirection2(3, 5, 5);
	lightDirection2.normalize();
	LightPtr light2(new DirectionalLight(Color::Green, lightDirection2));
	mLights.push_back(std::move(light2));
}

Color Scene::traceRay(const Ray& CameraRay)
{
	float maxTValue(9999);
	Intersection closestIntersection;

	for (const auto& shape : mShapes)
		shape->isIntersectingRay(CameraRay, maxTValue, closestIntersection);
	
	// If an object was intersected
	if (closestIntersection.object)
	{
		Color outputColor;
		const Material& surfaceMaterial(closestIntersection.object->getMaterial());
		const Vector3f& surfacePoint(closestIntersection.localGeometry.point);
		const Vector3f& surfaceNormal(closestIntersection.localGeometry.surfaceNormal);

		for (const auto& light : mLights)
		{
			// Get direction of light and compute h reflection
			const Vector3f& lightDirection(light->getLightDirectionFromPoint(surfacePoint));
			const Vector3f& h = computeBlinnSpecularReflection(lightDirection, -CameraRay.direction);

			// Get dot product of surface normal and h for specular lighting
			float specularFactor = std::max(dotProduct(surfaceNormal, h), 0.f);

			// Add glossiness expononent
			specularFactor = pow(specularFactor, surfaceMaterial.glossiness);

			// Get dot product of surface normal and light direction for diffuse lighting
			float diffuseFactor = std::max(dotProduct(surfaceNormal, lightDirection), 0.f);

			Color specularColor(light->getLightColor() * surfaceMaterial.specularColor);
			Color diffuseColor(light->getLightColor()  * surfaceMaterial.diffuseColor);

			outputColor = outputColor + ((diffuseColor * diffuseFactor) + (specularColor * specularFactor) + (diffuseColor * Color(30,30,30))); //fix the ambient at the end
		}

		return outputColor;
	}
	else
		return BACKGROUND_COLOR;
}


void Scene::renderScene()
{
	for (int y = 0; y < OUTPUT_RESOLUTION.y; y++)
	{
		for (int x = 0; x < OUTPUT_RESOLUTION.x; x++)
		{
			Ray ray = mCamera.generateRay(x, y);
			Color pixelColor = traceRay(ray);
			mOutputImage.setPixel(x, y, pixelColor);
		}
	}

	mOutputImage.writeImage();
}

Vector3f Scene::computeBlinnSpecularReflection(const Vector3f& LightDirection, const Vector3f& ViewerDirection)
{
	Vector3f reflection(LightDirection + ViewerDirection);
	reflection.normalize();
	return reflection;
}