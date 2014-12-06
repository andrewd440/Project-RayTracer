#include "KDTree.h"
#include "Intersection.h"

#include <algorithm>

KDTree::KDTree()
	: mRoot()
{

}


void KDTree::buildTree(const std::vector<std::unique_ptr<Shape>>& Shapes, uint32_t depth)
{
	for (const auto& Shape : Shapes)
		mRoot.ShapeList.push_back(Shape.get());

	mRoot.axis = 0;

	buildTreeHelper(mRoot, depth);
}


void KDTree::buildTreeHelper(KDNode& currentNode, uint32_t depth)
{
	if (depth == 0 || currentNode.ShapeList.size() <= 1)
		return;

	uint32_t dividingAxis = currentNode.axis;
	std::vector<Shape*>& currentShapes = currentNode.ShapeList;

	std::sort(currentShapes.begin(), currentShapes.end(), [dividingAxis](const Shape* lhs, const Shape* rhs) -> bool
	{
		return lhs->getBoundingBox().getCenter()[dividingAxis] < rhs->getBoundingBox().getCenter()[dividingAxis];
	});

	size_t ShapeListSize = currentNode.ShapeList.size();
	size_t medianShapeIndex = ShapeListSize / 2;
	Shape* medianShape = currentShapes[medianShapeIndex];
	float medianAxisValue = medianShape->getBoundingBox().getCenter()[dividingAxis];
	currentNode.splitValue = medianAxisValue;

	std::vector<Shape*> straddlingShapes;
	straddlingShapes.push_back(medianShape);
	currentNode.child[0] = std::unique_ptr<KDNode>(new KDNode());
	currentNode.child[1] = std::unique_ptr<KDNode>(new KDNode());

	// check for objects straddling the dividing axis, if so, add them to array
	for (int i = medianShapeIndex - 1; i >= 0; i--)
	{
		const AABB& currentBBox = currentShapes[i]->getBoundingBox();
		float axisRange = std::abs(currentBBox.min[dividingAxis] - currentBBox.max[dividingAxis]);
		float dividedRange = std::abs(currentBBox.min[dividingAxis] - medianAxisValue);
		if (axisRange > dividedRange)
			straddlingShapes.push_back(currentShapes[i]);
		else
		{
			currentNode.child[0]->ShapeList.push_back(currentShapes[i]);
		}
	}

	for (size_t i = medianShapeIndex + 1; i < ShapeListSize; i++)
	{
		const AABB& currentBBox = currentShapes[i]->getBoundingBox();
		float axisRange = currentBBox.max[dividingAxis] - currentBBox.min[dividingAxis];
		float dividedRange = currentBBox.max[dividingAxis] - medianAxisValue;
		if (axisRange > dividedRange)
			straddlingShapes.push_back(currentShapes[i]);
		else
		{
			currentNode.child[1]->ShapeList.push_back(currentShapes[i]);
		}
	}

	currentNode.child[0]->axis = currentNode.child[1]->axis = ++dividingAxis % 3;
	currentNode.ShapeList = straddlingShapes;

	buildTreeHelper(*currentNode.child[0], depth - 1);
	buildTreeHelper(*currentNode.child[1], depth - 1);
}

bool KDTree::isIntersectingRay(Ray ray, float* tValueOut, Intersection* intersectionOut)
{
	return visitNodesAgainstRay(&mRoot, ray, tValueOut, intersectionOut);
}

bool KDTree::visitNodesAgainstRay(KDNode* currentNode, Ray ray, float* tValueOut, Intersection* intersectionOut)
{
	if (currentNode == nullptr)
		return false;

	bool isIntersecting = false;

	for (Shape* Shape : currentNode->ShapeList)
		isIntersecting |= Shape->isIntersectingRay(ray, tValueOut, intersectionOut);

	// check which child to traverse first from axis split
	uint32_t axis = currentNode->axis;
	uint32_t first = ray.origin[axis] > currentNode->splitValue;

	if (ray.direction[axis] == 0.0f)
	{
		// Ray is parallel to the plane, visit only near side
		isIntersecting |= visitNodesAgainstRay(currentNode->child[first].get(), ray, tValueOut, intersectionOut);
	}
	else
	{
		// Find t value of intersection of ray with split plane
		float t = (currentNode->splitValue - ray.origin[axis]) / ray.direction[axis];

		// Check if ray straddles the splitting plane
		// If no tValueout is given, make one
		float maxTValue;
		if (tValueOut)
			maxTValue = *tValueOut;
		else
			maxTValue = std::numeric_limits<float>::max();

		if (0.0f <= t && t < maxTValue)
		{
			// Check for intersection in the near field
			isIntersecting |= visitNodesAgainstRay(currentNode->child[first].get(), ray, tValueOut, intersectionOut);
			isIntersecting |= visitNodesAgainstRay(currentNode->child[first ^ 1].get(), ray, tValueOut, intersectionOut);
		}
		else
		{
			// Just check near side
			isIntersecting |= visitNodesAgainstRay(currentNode->child[first].get(), ray, tValueOut, intersectionOut);
		}
	}

	return isIntersecting;
}