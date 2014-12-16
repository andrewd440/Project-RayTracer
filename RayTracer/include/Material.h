#pragma once

#include "Color.h"

#include <cassert>

/**
* Represents the lighting material for a specific surface 
*/
struct FMaterial
{
	/**
	* Constructs a Material with a specular, diffuse, ambient, and glossiness.
	* @param Specular - The specular color (default: (0.7, 0.7, 0.7))
	* @param Diffuse - The diffuse color (default: (0.7, 0.7, 0.7))
	* @param Ambient - The ambient color (default: (0.1, 0.1, 0.1))
	* @param Glossiness - The glossiness of the surface (default: 1.0)
	* @param Reflectivity - The reflectiveness of the surface [0-1] (default: 0.0)
	*/
	FMaterial(	FColor Specular = FColor(0.7f, 0.7f, 0.7f), 
				FColor Diffuse = FColor(0.7f, 0.7f, 0.7f), 
				FColor Ambient = FColor(0.1f, 0.1f, 0.1f), 
				float Glossiness = 1.0f, 
				float Reflectivity = 0.0f)
		: specularColor(Specular)
		, diffuseColor(Diffuse)
		, ambientColor(Ambient)
		, glossiness(Glossiness)
		, reflectivity(Reflectivity)
	{
		// Reflectivity must be between 0 and 1
		assert(reflectivity <= 1 && reflectivity >= 0);
	}

	FColor specularColor;
	FColor diffuseColor;
	FColor ambientColor;
	float glossiness;
	float reflectivity;
};

