#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "LocalGeometry.h"
#include "Vector3.inl"
#include "DirectionalLight.h"

#include <algorithm>

namespace
{
	const Vector2i OUTPUT_RESOLUTION(1000, 700);
	const Color BACKGROUND_COLOR(Color::Black);
	const Color GLOBAL_AMBIENT_COLOR(100, 100, 100);
}

Scene::Scene()
	: mCamera(Vector3f(0, 0, 0), Vector3f(0,0,1), 65, OUTPUT_RESOLUTION)
	, mOutputImage("RenderedScene", OUTPUT_RESOLUTION)
	, mShapes()
	, mLights()
{
	buildScene();
}

//////////////////////////////////////////////////////////////////////////////////////////////

void Scene::buildScene()
{
	Material redMaterial(Color(150, 150, 150), Color(255, 0, 0), Color(25, 25, 25), 50, .9f);
	ShapePtr shape(new Sphere(Vector3f(0, 0, 17), 2.f, redMaterial));
	mShapes.push_back(std::move(shape));

	Material greenMaterial(Color(150, 150, 150), Color::Green, Color(25, 25, 25), 50, .9f);
	ShapePtr shape2(new Sphere(Vector3f(0, 4, 17), 1.5f, greenMaterial));
	mShapes.push_back(std::move(shape2));

	Material blueMaterial(Color(150, 150, 150), Color::Blue, Color(25, 25, 25), 50, .9f);
	ShapePtr shape3(new Sphere(Vector3f(0, -4, 17), 1.5f, blueMaterial));
	mShapes.push_back(std::move(shape3));

	Material redBlueMaterial(Color(150, 150, 150), Color(255, 255, 0), Color(25, 25, 25), 50, .9f);
	ShapePtr shape4(new Sphere(Vector3f(4, 0, 17), 1.5f, redBlueMaterial));
	mShapes.push_back(std::move(shape4));

	Material whiteMaterial(Color(150, 150, 150), Color(255, 255, 255), Color(25, 25, 25), 50, .9f);
	ShapePtr shape5(new Sphere(Vector3f(-4, 0, 17), 1.5f, whiteMaterial));
	mShapes.push_back(std::move(shape5));

	Vector3f lightDirection(-1, -1, -3);
	LightPtr light(new DirectionalLight(Color::White, lightDirection));
	mLights.push_back(std::move(light));

	Vector3f lightDirection2(.5, .5, 1);
	LightPtr light2(new DirectionalLight(Color::White, lightDirection2));
	mLights.push_back(std::move(light2));
}

//////////////////////////////////////////////////////////////////////////////////////////////

Color Scene::traceRay(const Ray& CameraRay, int Depth)
{
	if (Depth < 1)
		return BACKGROUND_COLOR;

	float maxTValue(9999);
	Intersection closestIntersection;

	for (const auto& shape : mShapes)
		shape->isIntersectingRay(CameraRay, maxTValue, closestIntersection);
	
	// If an object was intersected
	if (closestIntersection.object)
	{
		Color outputColor;

		// Get the surface material, point, and normal
		const Material& surfaceMaterial(closestIntersection.object->getMaterial());
		const Vector3f& surfacePoint(closestIntersection.localGeometry.point);
		const Vector3f& surfaceNormal(closestIntersection.localGeometry.surfaceNormal);

		for (const auto& light : mLights)
		{
			// Get direction of light and compute h reflection
			const Ray& rayToLight(light->getRayToLight(surfacePoint));
			const Vector3f& lightDirection(rayToLight.direction);
			const Vector3f& h = computeBlinnSpecularReflection(rayToLight.direction, -CameraRay.direction);

			// If an object is in the way of the light, skip lighting for that light
			if (isInShadow(closestIntersection.object, rayToLight))
				continue;

			// Get dot product of surface normal and h for specular lighting
			float specularFactor = std::max(dotProduct(surfaceNormal, h), 0.f);

			// Add glossiness expononent
			specularFactor = pow(specularFactor, surfaceMaterial.glossiness);

			// Get dot product of surface normal and light direction for diffuse lighting
			float diffuseFactor = std::max(dotProduct(surfaceNormal, lightDirection), 0.f);

			// Combine material color and light color for diffuse and specular
			Color specularColor(light->getLightColor() * surfaceMaterial.specularColor);
			Color diffuseColor(light->getLightColor()  * surfaceMaterial.diffuseColor);

			// Add diffuse and specular contributions to total
			outputColor += ((diffuseColor * diffuseFactor) + (specularColor * specularFactor));

			// Add mirror reflection contributions
			Ray reflectionRay(surfacePoint, computeMirriorReflection(lightDirection, surfaceNormal));
			outputColor += traceRay(reflectionRay, Depth - 1) * outputColor * surfaceMaterial.reflectivity;
		}

		// return computed color totals with ambient contribution
		return outputColor + (GLOBAL_AMBIENT_COLOR * surfaceMaterial.ambientColor);
	}
	else
		return BACKGROUND_COLOR;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void Scene::renderScene()
{
	for (int y = 0; y < OUTPUT_RESOLUTION.y; y++)
	{
		for (int x = 0; x < OUTPUT_RESOLUTION.x; x++)
		{
			Ray ray = mCamera.generateRay(x, y);
			Color pixelColor = traceRay(ray,4);
			mOutputImage.setPixel(x, y, pixelColor);
		}
	}

	mOutputImage.writeImage();
}

//////////////////////////////////////////////////////////////////////////////////////////////

Vector3f Scene::computeBlinnSpecularReflection(const Vector3f& LightDirection, const Vector3f& ViewerDirection)
{
	Vector3f reflection(LightDirection + ViewerDirection);
	reflection.normalize();
	return reflection;
}

//////////////////////////////////////////////////////////////////////////////////////////////

bool Scene::isInShadow(Shape* ReferenceShape, const Ray& LightRay) const
{
	for (const auto& shape : mShapes)
	{
		// If the object is not the reference one and intersects the light
		if (shape.get() != ReferenceShape && shape->isIntersectingRay(LightRay))
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Vector3f Scene::computeMirriorReflection(const Vector3f& LightDirection, const Vector3f& SurfaceNormal) const
{
	float lightDotNormal = dotProduct(SurfaceNormal, LightDirection);
	Vector3f reflectionDirection(2 * lightDotNormal * SurfaceNormal - LightDirection);

	return reflectionDirection;
}