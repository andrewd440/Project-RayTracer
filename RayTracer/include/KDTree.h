#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "Drawable.h"

struct KDNode
{
	std::unique_ptr<KDNode> child[2]; // 0 = near, 1 = far
	int axis; // x, y, or z splitting axis
	float splitValue; // value on the splitting axis
	std::vector<IDrawable*> PrimitiveList; // linked list of Primitives within this node
};

class KDTree
{
public:
	KDTree();

	void buildTree(const std::vector<std::unique_ptr<IDrawable>>& Primitives, uint32_t depth);

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
	bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr);

private:
	void buildTreeHelper(KDNode& currentNode, uint32_t depth);
	bool visitNodesAgainstRay(KDNode* currentNode, FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr);

private:
	KDNode mRoot;
};