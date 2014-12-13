#pragma once
#include "Drawable.h"
#include "Triangle.h"

#include <vector>
#include <memory>
#include <string>

/**
* A 3D triangle mesh
*/
class FMesh : public IDrawable
{
public:
	/**
	* Default constructor. Creates an empty mesh.
	*/
	FMesh();

	/**
	* Creates a triangle mesh from vertices and faces in a
	* .obj file.
	* @param ModelFilepath The file path of the model.
	*/
	FMesh(const std::string& ModelFilepath);

	~FMesh();

	/**
	* Checks if a ray intersects any triangles in the mesh.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the Primitive.
	*/
	bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr) override;

private:
	void ConstructAABB() override;

	/* Reads a .obj model into this object */
	void ReadModel(const std::string& ModelFilepath);

private:
	std::vector<FTriangle> mTriangles;
};

