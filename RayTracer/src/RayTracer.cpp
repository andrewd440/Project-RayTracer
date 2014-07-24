// RayTracer.cpp : Defines the entry point for the console application.
//

#include "Vector3.h"
#include "Vector3.inl"
#include "Ray.h"
#include <iostream>


int main()
{
	std::cout << sizeof(Ray) << std::endl;
	std::cout << sizeof(Vector3f) << std::endl;
	std::cout << sizeof(char) << std::endl;
	std::cout << sizeof(float) << std::endl;
	return 0;
}

