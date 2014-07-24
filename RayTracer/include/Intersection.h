#pragma once
#include "LocalGeometry.h"

class Shape;

/**
* Struct for describing the properties of an intersection 
* with a primative at a point
*/
struct Intersection
{
	/**
	* Default constructor.
	*/
	Intersection()
		: object(nullptr)
		, localGeometry()
	{
	}

	/**
	* Construct Intersection from the object of the intersection and the points' local
	* geometry properties
	* @param intersectedObject - object of the intersection
	* @param geometryAtIntersection - the geometry of the point of intersection
	*/
	Intersection(Shape& intersectedObject, LocalGeometry geometryAtIntersection)
		: object(&intersectedObject)
		, localGeometry(geometryAtIntersection)
	{
	}
		
	Shape* object; /* The primative of the intersection */
	LocalGeometry localGeometry; /* The geometry at the point of intersection */
};

