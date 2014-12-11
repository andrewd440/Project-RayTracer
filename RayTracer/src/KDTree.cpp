#include "KDTree.h"
#include "Intersection.h"

#include <algorithm>

KDTree::KDTree()
	: mRoot()
{

}


void KDTree::buildTree(const std::vector<std::unique_ptr<IPrimitive>>& Primitives, uint32_t depth)
{
	for (const auto& Primitive : Primitives)
		mRoot.PrimitiveList.push_back(Primitive.get());

	mRoot.axis = 0;

	buildTreeHelper(mRoot, depth);
}


void KDTree::buildTreeHelper(KDNode& currentNode, uint32_t depth)
{
	if (depth == 0 || currentNode.PrimitiveList.size() <= 1)
		return;

	uint32_t dividingAxis = currentNode.axis;
	std::vector<IPrimitive*>& currentPrimitives = currentNode.PrimitiveList;

	std::sort(currentPrimitives.begin(), currentPrimitives.end(), [dividingAxis](const IPrimitive* lhs, const IPrimitive* rhs) -> bool
	{
		return lhs->getBoundingBox().GetCenter()[dividingAxis] < rhs->getBoundingBox().GetCenter()[dividingAxis];
	});

	size_t PrimitiveListSize = currentNode.PrimitiveList.size();
	size_t medianPrimitiveIndex = PrimitiveListSize / 2;
	IPrimitive* medianPrimitive = currentPrimitives[medianPrimitiveIndex];
	float medianAxisValue = medianPrimitive->getBoundingBox().GetCenter()[dividingAxis];
	currentNode.splitValue = medianAxisValue;

	std::vector<IPrimitive*> straddlingPrimitives;
	straddlingPrimitives.push_back(medianPrimitive);
	currentNode.child[0] = std::unique_ptr<KDNode>(new KDNode());
	currentNode.child[1] = std::unique_ptr<KDNode>(new KDNode());

	// check for objects straddling the dividing axis, if so, add them to array
	for (int i = medianPrimitiveIndex - 1; i >= 0; i--)
	{
		const AABB& currentBBox = currentPrimitives[i]->getBoundingBox();
		float axisRange = std::abs(currentBBox.min[dividingAxis] - currentBBox.max[dividingAxis]);
		float dividedRange = std::abs(currentBBox.min[dividingAxis] - medianAxisValue);
		if (axisRange > dividedRange)
			straddlingPrimitives.push_back(currentPrimitives[i]);
		else
		{
			currentNode.child[0]->PrimitiveList.push_back(currentPrimitives[i]);
		}
	}

	for (size_t i = medianPrimitiveIndex + 1; i < PrimitiveListSize; i++)
	{
		const AABB& currentBBox = currentPrimitives[i]->getBoundingBox();
		float axisRange = currentBBox.max[dividingAxis] - currentBBox.min[dividingAxis];
		float dividedRange = currentBBox.max[dividingAxis] - medianAxisValue;
		if (axisRange > dividedRange)
			straddlingPrimitives.push_back(currentPrimitives[i]);
		else
		{
			currentNode.child[1]->PrimitiveList.push_back(currentPrimitives[i]);
		}
	}

	currentNode.child[0]->axis = currentNode.child[1]->axis = ++dividingAxis % 3;
	currentNode.PrimitiveList = straddlingPrimitives;

	buildTreeHelper(*currentNode.child[0], depth - 1);
	buildTreeHelper(*currentNode.child[1], depth - 1);
}

bool KDTree::IsIntersectingRay(FRay ray, float* tValueOut, FIntersection* intersectionOut)
{
	return visitNodesAgainstRay(&mRoot, ray, tValueOut, intersectionOut);
}

bool KDTree::visitNodesAgainstRay(KDNode* currentNode, FRay ray, float* tValueOut, FIntersection* intersectionOut)
{
	if (currentNode == nullptr)
		return false;

	bool isIntersecting = false;

	for (IPrimitive* primitive : currentNode->PrimitiveList)
		isIntersecting |= primitive->IsIntersectingRay(ray, tValueOut, intersectionOut);

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