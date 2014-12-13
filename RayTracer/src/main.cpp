// RayTracer.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <istream>
#include <cstdlib>
#include <time.h>
#include "Matrix4.h"

#include "Camera.h"
#include "Ray.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Scene.h"

#include "Mesh.h"

int main()
{
	std::srand(time(nullptr));
	clock_t t1, t2;
	t1 = clock();
	std::filebuf fb;
	if (fb.open("SceneConfig.scn", std::ios::in))
	{
		std::istream configStream(&fb);
		FScene scene;
		scene.BuildScene(configStream);
		scene.RenderScene();
	}
	fb.close();
	t2 = clock();
	std::cout << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << std::endl;

	return 0;
}