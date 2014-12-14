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
	* @param Specular - The specular color
	* @param Diffuse - The diffuse color
	* @param Ambient - The ambient color
	* @param Glossiness - The glossiness of the surface
	* @param Reflectivity - The reflectiveness of the surface [0-1]
	*/
	FMaterial(FColor Specular = FColor(0.7f, 0.7f, 0.7f), FColor Diffuse = FColor(0.7f, 0.7f, 0.7f), FColor Ambient = FColor(0.1f, 0.1f, 0.1f), float Glossiness = 1.0f, float Reflectivity = 0.0f)
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

