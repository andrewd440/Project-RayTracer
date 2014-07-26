// RayTracer.cpp : Defines the entry point for the console application.
//

#include "Camera.h"
#include <iostream>
#include "RayTracer.h"
#include <fstream>
#include <sstream>

using namespace std;

const Vector2i SCREENRES(1000, 600);

int main()
{
	Camera camera(Vector3f(0, 0, 0), Vector3f(0, 0, 1), 85, Vector2i(SCREENRES.x, SCREENRES.y));
	RayTracer rayTracer;

	// Set up the output file (TODO: the filename should probably be a commandline parameter)
	std::ostringstream headerStream;
	headerStream << "P6\n";
	headerStream << SCREENRES.x << ' ' << SCREENRES.y << '\n';
	headerStream << "255\n";
	std::ofstream fileStream("out.ppm", std::ios::out | std::ios::binary);
	fileStream << headerStream.str();

	unsigned char r, g, b;

	for (int i = 0; i < SCREENRES.y; i++)
	{
		for (int j = 0; j < SCREENRES.x; j++)
		{
			Ray ray = camera.generateRay(j, i);
			bool hit = rayTracer.trace(ray);
			if (hit)
			{
				r = static_cast<unsigned char>(1 * 255.0f);
				g = static_cast<unsigned char>(0 * 255.0f);
				b = static_cast<unsigned char>(0 * 255.0f);
			}
			else
			{
				r = static_cast<unsigned char>(0 * 255.0f);
				g = static_cast<unsigned char>(0 * 255.0f);
				b = static_cast<unsigned char>(0 * 255.0f);
			}
			fileStream << r << g << b;
		}
	}

	// Tidy up (probably unnecessary)
	fileStream.flush();
	fileStream.close();

	return 0;
}