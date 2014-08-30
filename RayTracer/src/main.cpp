// RayTracer.cpp : Defines the entry point for the console application.
//

#include "Scene.h"
#include <iostream>
#include <istream>

using namespace std;

int main()
{
	std::filebuf fb;
	if (fb.open("SceneConfig.scn", std::ios::in))
	{
		std::istream configStream(&fb);
		Scene scene;
		scene.buildScene(configStream);
		scene.renderScene();
	}

	fb.close();

	return 0;
}