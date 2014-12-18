#pragma once
#include "Drawable.h"
#include "Triangle.h"
#include "Texture.h"

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
	FMesh(const FMaterial& Material = FMaterial());

	/**
	* Creates a triangle mesh from vertices and faces in a
	* .obj file.
	* @param ModelFilepath The file path of the model.
	*/
	FMesh(const std::string& ModelFilepath, const FMaterial& Material = FMaterial());

	/**
	* Creates a texture mapped triangle mesh from vertices and faces in a
	* .obj file.
	* @param ModelFilepath The file path of the model.
	* @param Texture for the model.
	*/
	FMesh(const std::string& ModelFilepath, FTexture& Texture);

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

	void SetMaterial(const FMaterial& Material) override;

	/**
	* Retrieves the default material for the mesh.
	* @return The material
	*/
	FMaterial GetMaterial(Vector3f SurfacePoint) override;

private:
	void ConstructAABB(Vector3f Min = Vector3f(), Vector3f Max = Vector3f()) override;

	/* Reads a .obj model into this object and returns an AABB for the model */
	void ReadModel(const std::string& ModelFilepath);

private:
	std::vector<std::unique_ptr<FTriangle>> mTriangles; /* All triangles in the mesh */
};

