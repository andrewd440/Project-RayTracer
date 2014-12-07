#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "Primitive.h"

struct KDNode
{
	std::unique_ptr<KDNode> child[2]; // 0 = near, 1 = far
	int axis; // x, y, or z splitting axis
	float splitValue; // value on the splitting axis
	std::vector<Primitive*> PrimitiveList; // linked list of Primitives within this node
};

class KDTree
{
public:
	KDTree();

	void buildTree(const std::vector<std::unique_ptr<Primitive>>& Primitives, uint32_t depth);

	/**
	* Checks if a ray intersects an object in the kdtree.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the Primitive.
	*/
	bool IsIntersectingRay(Ray Ray, float* tValueOut = nullptr, Intersection* IntersectionOut = nullptr);

private:
	void buildTreeHelper(KDNode& currentNode, uint32_t depth);
	bool visitNodesAgainstRay(KDNode* currentNode, Ray Ray, float* tValueOut = nullptr, Intersection* IntersectionOut = nullptr);

private:
	KDNode mRoot;
};