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
	/** 
	* Node for the bounding volume hierarchy used for
	* meshes. The nodes should solely own the objects
	* within it, so they are destroyed on node destruction.
	*/
	struct FBVHNode
	{
		AABB BoundingVolume;
		std::unique_ptr<FBVHNode> Child[2];
		std::vector<std::unique_ptr<FTriangle>> Objects;
		uint8_t SplitAxis;
	};

	void ConstructAABB(Vector3f Min = Vector3f(), Vector3f Max = Vector3f()) override;

	/* Reads a .obj model into this object and returns an AABB for the model */
	void ReadModel(const std::string& ModelFilepath);

	void ConstructBVH(FBVHNode& Node, const uint8_t Depth);

	void ConstructBoundingVolume(FBVHNode& Node);

	bool TraverseBVH(FBVHNode& Node, FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr);

private:
	std::vector<std::unique_ptr<FTriangle>> mTriangles; /* All triangles in the mesh */
	FBVHNode mBVHRoot; /* Root node for the mesh BVH */
};

