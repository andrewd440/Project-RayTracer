#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "LocalGeometry.h"
#include "Vector3.inl"
#include "DirectionalLight.h"

#include <algorithm>

const Vector2i OUTPUT_RESOLUTION(800, 500);
const Color BACKGROUND_COLOR(0, 0, 0);
const Color GLOBAL_AMBIENT_COLOR(100, 100, 100);

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
	Material redMaterial(Color(100,100,100), Color(210,40,50), Color(25,25,25), 64);
	ShapePtr shape(new Sphere(Vector3f(0, 0, 20), 4, redMaterial));
	mShapes.push_back(std::move(shape));

	ShapePtr shape2(new Sphere(Vector3f(3, 3, 13), 2, redMaterial));
	mShapes.push_back(std::move(shape2));

	Vector3f lightDirection(-3, -3, 5);
	LightPtr light(new DirectionalLight(Color::White, lightDirection));
	mLights.push_back(std::move(light));

	Vector3f lightDirection2(0, -1, 1);
	LightPtr light2(new DirectionalLight(Color::White, lightDirection2));
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

			// If an object is in the way of the light, skip lighting for that light
			if (isInShadow(closestIntersection.object, Ray(surfacePoint, lightDirection)))
				continue;

			// Get dot product of surface normal and h for specular lighting
			float specularFactor = std::max(dotProduct(surfaceNormal, h), 0.f);

			// Add glossiness expononent
			specularFactor = pow(specularFactor, surfaceMaterial.glossiness);

			// Get dot product of surface normal and light direction for diffuse lighting
			float diffuseFactor = std::max(dotProduct(surfaceNormal, lightDirection), 0.f);

			Color specularColor(light->getLightColor() * surfaceMaterial.specularColor);
			Color diffuseColor(light->getLightColor()  * surfaceMaterial.diffuseColor);

			outputColor = outputColor + ((diffuseColor * diffuseFactor) + (specularColor * specularFactor));
		}

		return outputColor + (GLOBAL_AMBIENT_COLOR * surfaceMaterial.ambientColor); //fix the ambient at the end
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

bool Scene::isInShadow(Shape* ReferenceShape, const Ray& LightRay) const
{
	for (const auto& shape : mShapes)
	{
		// If the object is not the reference one and intersects the light
		if (shape.get() != ReferenceShape && shape->isIntersectingRay(LightRay))
		{
			return true;
		}
	}

	return false;
}