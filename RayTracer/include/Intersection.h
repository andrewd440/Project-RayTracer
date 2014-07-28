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
	* @param IntersectedObject - object of the intersection
	* @param GeometryAtIntersection - the geometry of the point of intersection
	*/
	Intersection(Shape& IntersectedObject, LocalGeometry GeometryAtIntersection)
		: object(&IntersectedObject)
		, localGeometry(GeometryAtIntersection)
	{
	}
		
	Shape* object; /* The primative of the intersection */
	LocalGeometry localGeometry; /* The geometry at the point of intersection */
};

