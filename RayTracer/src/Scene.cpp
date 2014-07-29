#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "LocalGeometry.h"
#include "Vector3.inl"

const Vector2i OUTPUT_RESOLUTION(1920, 1080);

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
}

Color Scene::traceRay(const Ray& CameraRay)
{
	float tValue;
	Intersection inters;
	if (mShapes[0]->isIntersectingRay(CameraRay, tValue, inters))
		return Color(255, 0, 0);
	return Color(0, 0, 0);
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