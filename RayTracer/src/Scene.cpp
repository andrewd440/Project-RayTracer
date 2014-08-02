#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "LocalGeometry.h"
#include "Vector3.inl"
#include "DirectionalLight.h"

#include <iostream>
#include <algorithm>
#include <string>

namespace
{
	const Vector2i OUTPUT_RESOLUTION(1000, 700);
}

Scene::Scene()
	: mOutputImage("RenderedScene", OUTPUT_RESOLUTION)
	, mBackgroundColor(Color::Black)
	, mGlobalAmbient(100, 100, 100)
	, mCamera(Vector3f(0, 0, 0), Vector3f(0, 0, 1), 65, OUTPUT_RESOLUTION)
	, mShapes()
	, mLights()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////

void Scene::buildScene(std::istream& in)
{
	std::string string;
	in >> string;

	while (in.good())
	{
		if (string == "Camera")
		{
			Vector3f position;
			Vector3f direction;
			float FOV;

			in >> string;
			in >> position.x >> position.y >> position.z;
			in >> string;
			in >> direction.x >> direction.y >> direction.z;
			in >> string;
			in >> FOV;

			mCamera.setPosition(position);
			mCamera.setDirection(direction);
			mCamera.setFOV(FOV);
		}
		else if (string == "DirectionalLight")
		{
			Color color;
			Vector3f direction;

			in >> string;
			in >> color.r >> color.g >> color.b;
			in >> string;
			in >> direction.x >> direction.y >> direction.z;

			mLights.push_back(LightPtr(new DirectionalLight(color, direction)));
		}
		else if (string == "Sphere")
		{
			Vector3f center;
			float radius;
			Color specular, diffuse, ambient;
			float specualarExponent, reflectivity;

			in >> string;
			in >> center.x >> center.y >> center.z;
			in >> string;
			in >> radius;
			in >> string;
			in >> specular.r >> specular.g >> specular.b;
			in >> string;
			in >> diffuse.r >> diffuse.g >> diffuse.b;
			in >> string;
			in >> ambient.r >> ambient.g >> ambient.b;
			in >> string;
			in >> specualarExponent;
			in >> string;
			in >> reflectivity;

			Material material(specular, diffuse, ambient, specualarExponent, reflectivity);

			mShapes.push_back(ShapePtr(new Sphere(center, radius, material)));
		}
		in >> string;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

Color Scene::traceRay(const Ray& CameraRay, int Depth)
{
	if (Depth < 1)
		return mBackgroundColor;

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
		return outputColor + (mGlobalAmbient * surfaceMaterial.ambientColor);
	}
	else
		return mBackgroundColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void Scene::renderScene()
{
	for (int y = 0; y < OUTPUT_RESOLUTION.y; y++)
	{
		for (int x = 0; x < OUTPUT_RESOLUTION.x; x++)
		{
			Ray ray = mCamera.generateRay(x, y);
			Color pixelColor = traceRay(ray,20);
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