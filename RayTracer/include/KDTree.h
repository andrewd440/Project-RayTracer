#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "Drawable.h"

struct KDNode
{
	std::unique_ptr<KDNode> Child[2]; // 0 = near, 1 = far
	float SplitValue; // value on the splitting axis
	std::vector<IDrawable*> ObjectList; // list of Primitives within this node
	uint8_t Axis; // x, y, or z splitting axis
};

class KDTree
{
public:
	KDTree();

	/**
	* Builds a KD-tree from a list of objects. Straddling objects are placed in the
	* current node as the tree is constructed.
	* @param Objects to build the tree from.
	* @param Depth Max depth of the tree.
	* @param MinObjectsPerNode Minimum objects for a node before tree construction ends.
	*/
	void BuildTree(const std::vector<std::unique_ptr<IDrawable>>& Objects, uint32_t Depth, uint32_t MinObjectsPerNode);

	/**
	* Checks if a ray intersects an object in the kdtree.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer. If an intersection pointer is not given, it is assumed
	* that a single intersection is being checked for, so the function will return true with the first valid
	* intersection.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the Primitive.
	*/
	bool IsIntersectingRay(FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr);

private:
	void BuildTreeHelper(KDNode& currentNode, uint32_t depth);
	bool VisitNodesAgainstRay(KDNode* currentNode, FRay Ray, float* tValueOut = nullptr, FIntersection* IntersectionOut = nullptr);

private:
	KDNode mRoot;
	uint32_t mMinObjectsPerNode;
};