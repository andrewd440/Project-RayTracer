#pragma once
#include "Ray.h"
#include "Sphere.h"

class RayTracer
{
public:
	RayTracer();
	
	////////////////////////////////////
	// change bool to void after testing
	////////////////////////////////////
	bool trace(const Ray& ray);

private:
	////////////////////
	/// Remove after test
	////////////////////
	Sphere sphere;
};

