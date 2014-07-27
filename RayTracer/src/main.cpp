// RayTracer.cpp : Defines the entry point for the console application.
//

#include "Camera.h"
#include <iostream>
#include "RayTracer.h"
#include "Image.h"

using namespace std;

const Vector2i SCREENRES(1000, 600);

int main()
{
	Camera camera(Vector3f(0, 0, 0), Vector3f(0, 0, 1), 85, Vector2i(SCREENRES.x, SCREENRES.y));
	RayTracer rayTracer;
	Image img("Output", SCREENRES);

	for (int i = 0; i < 1000; i++)
		for (int j = 0; j < 600; j++)
			if (rayTracer.trace(camera.generateRay(i, j)))
				img.setPixel(i, j, Color(255, 0, 0));
			

	img.writeImage();

	return 0;
}