#pragma once

#include <cstdint>
#include <vector>

#include "Shape.h"

struct KDNode;

class KDTree
{
public:
	KDTree(const std::vector<Shape>& shapes, uint32_t depth);
	~KDTree();

	void buildTree(std::vector<Shape>& shapes, uint32_t depth = mMaxDepth);

private:
	KDNode mRoot;
	uint32_t mMaxDepth;
};

struct KDNode
{
	KDNode* child[2]; // 0 = near, 1 = far
	char axis; // x, y, or z splitting axis
	float splitValue; // value on the splitting axis
	std::vector<Shape*> shapeList; // linked list of shapes within this node
};