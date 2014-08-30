#include "KDTree.h"


KDTree::KDTree(const std::vector<Shape>& shapes, uint32_t depth)
	: mRoot()
	, mMaxDepth(depth)
{
	for (Shape& shape : shapes)
		mRoot.shapeList.push_back(shape);
}


KDTree::~KDTree()
{
}
