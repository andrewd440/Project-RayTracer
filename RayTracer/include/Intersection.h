#pragma once

class Primitive;

/**
* Struct for describing the properties of an intersection 
* with a primative at a point
*/
struct Intersection
{
	/**
	* Default Constructor 
	*/
	inline Intersection()
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
	inline Intersection(Primitive& IntersectedObject, Vector3f IntersectionPoint, Vector3f SurfaceNormal)
		: object(&IntersectedObject)
		, point(IntersectionPoint)
		, normal(SurfaceNormal)
	{
	}

		
	Primitive* object;				/* The primative of the intersection */
	Vector3f point;				/* Point on the surface of the geometry */
	Vector3f normal;			/* Surface normal at the point on the geometry */
};

