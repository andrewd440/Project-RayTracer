#pragma once

class IDrawable;

/**
* Struct for describing the properties of an intersection 
* with a primative at a point
*/
struct FIntersection
{
	/**
	* Default Constructor 
	*/
	inline FIntersection()
		: object(nullptr)
		, point()
		, normal()
	{
	}

	/**
	* Construct Intersection from the object of the intersection and the points' local
	* geometry properties
	* @param IntersectedObject Object of the intersection
	* @param IntersectionPoint Point of intersection
	* @param SurfaceNormal Normal at the intersection of the object
	*/
	inline FIntersection(IDrawable& IntersectedObject, Vector3f IntersectionPoint, Vector3f SurfaceNormal)
		: object(&IntersectedObject)
		, point(IntersectionPoint)
		, normal(SurfaceNormal)
	{
	}

		
	IDrawable* object;				/* The primative of the intersection */
	Vector3f point;				/* Point on the surface of the geometry */
	Vector3f normal;			/* Surface normal at the point on the geometry */
};

