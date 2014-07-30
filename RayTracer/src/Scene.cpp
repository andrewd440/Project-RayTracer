#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "LocalGeometry.h"
#include "Vector3.inl"
#include "DirectionalLight.h"

const Vector2i OUTPUT_RESOLUTION(1600, 1000);
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
	ShapePtr shape(new Sphere(Vector3f(0, 0, 20), 2));
	mShapes.push_back(std::move(shape));

	Vector3f lightDirection(-3, -5, 5);
	lightDirection.normalize();
	LightPtr light(new DirectionalLight(Color(255, 255, 255), lightDirection));
	mLights.push_back(std::move(light));
}

Color Scene::traceRay(const Ray& CameraRay)
{
	float maxTValue(1000);
	Intersection closestIntersection;
	bool hasIntersection = false;

	for (const auto& shape : mShapes)
		hasIntersection |= shape->isIntersectingRay(CameraRay, maxTValue, closestIntersection);
	
	if (hasIntersection)
	{
		Vector3f h = computeBlinnSpecularReflection(mLights[0]->getLightDirectionFromPoint(closestIntersection.localGeometry.point), -CameraRay.direction);
		float nDoth = dotProduct(closestIntersection.localGeometry.surfaceNormal, h);
		nDoth = pow(nDoth, 16);
		if (nDoth < 0) nDoth = 0;
		float nDotL = dotProduct(closestIntersection.localGeometry.surfaceNormal, mLights[0]->getLightDirectionFromPoint(closestIntersection.localGeometry.point));
		if (nDotL < 0) nDotL = 0;
		return Color((255 * nDoth + 0 * nDotL) / 2.f, (255 * nDoth + 255 * nDotL) / 2.f, (255 * nDoth + 0 * nDotL) / 2.f);
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