#pragma once

#include "Color.h"

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
	*/
	Material(Color Specular, Color Diffuse, Color Ambient, float Glossiness)
		: specularColor(Specular)
		, diffuseColor(Diffuse)
		, ambientColor(Ambient)
		, glossiness(Glossiness)
	{
	}

	Color specularColor;
	Color diffuseColor;
	Color ambientColor;
	float glossiness;
};

