#pragma once
#include "Vector3.h"

/**
* Struct of describing geometry properties of an object at a point.
*/
struct LocalGeometry
{
	/**
	* Default constructor.
	* point = normal = zero vector
	*/
	LocalGeometry()
		: point()
		, surfaceNormal()
	{
	}

	/**
	* Construct local geometry from a surface point and normal
	* @param Point - a point on a geometry
	* @param Normal - normalized surface normal at the pointVector
	*/
	LocalGeometry(Vector3f Point, Vector3f Normal)
		: point(Point)
		, surfaceNormal(Normal)
	{
	}

	Vector3f point;				/* Point on the surface of the geometry */
	Vector3f surfaceNormal;		/* Surface normal at the point on the geometry */
};

