#pragma once

#include "Color.h"

#include <cassert>

/**
* Represents the lighting material for a specific surface 
*/
struct Material
{
	/**
	* Constructs a Material with a specular, diffuse, ambient, and glossiness.
	* @param Specular - The specular color
	* @param Diffuse - The diffuse color
	* @param Ambient - The ambient color
	* @param Glossiness - The glossiness of the surface
	* @param Reflectivity - The reflectiveness of the surface [0-1]
	*/
	Material(Color Specular, Color Diffuse, Color Ambient, float Glossiness, float Reflectivity)
		: specularColor(Specular)
		, diffuseColor(Diffuse)
		, ambientColor(Ambient)
		, glossiness(Glossiness)
		, reflectivity(Reflectivity)
	{
		// Reflectivity must be between 0 and 1
		assert(reflectivity <= 1 && reflectivity >= 0);
	}

	Color specularColor;
	Color diffuseColor;
	Color ambientColor;
	float glossiness;
	float reflectivity;
};

