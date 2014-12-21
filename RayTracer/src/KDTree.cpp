#include "KDTree.h"
#include "Intersection.h"

#include <algorithm>

KDTree::KDTree()
	: mRoot()
{

}


void KDTree::BuildTree(const std::vector<std::unique_ptr<IDrawable>>& Primitives, uint32_t depth, uint32_t MinObjectsPerNode)
{
	for (const auto& Primitive : Primitives)
		mRoot.ObjectList.push_back(Primitive.get());

	mRoot.Axis = 0;

	BuildTreeHelper(mRoot, depth, MinObjectsPerNode);
}


void KDTree::BuildTreeHelper(KDNode& CurrentNode, uint32_t Depth, uint32_t MinObjectsPerNode)
{
	if (Depth == 0 || CurrentNode.ObjectList.size() <= MinObjectsPerNode)
		return;

	const uint32_t& DividingAxis = CurrentNode.Axis;
	std::vector<IDrawable*>& CurrentObjects = CurrentNode.ObjectList;

	// sort all objects by splitting axis
	std::sort(CurrentObjects.begin(), CurrentObjects.end(), [DividingAxis](const IDrawable* lhs, const IDrawable* rhs) -> bool
	{
		return lhs->GetWorldAABB().GetCenter()[DividingAxis] < rhs->GetWorldAABB().GetCenter()[DividingAxis];
	});

	const size_t PrimitiveListSize = CurrentNode.ObjectList.size();
	size_t DividingObjectIndex = PrimitiveListSize / 2;
	IDrawable* DividingObject = CurrentObjects[DividingObjectIndex];

	AABB DividingAABB = DividingObject->GetWorldAABB();
	
	// offset the dividing axis value to just after the median objects AABB
	const float DividingAxisValue = DividingAABB.GetCenter()[DividingAxis] + DividingAABB.GetDeminsions()[DividingAxis] + 0.1f;
	CurrentNode.SplitValue = DividingAxisValue;

	std::vector<IDrawable*> StraddlingPrimitives;
	CurrentNode.Child[0] = std::unique_ptr<KDNode>(new KDNode());
	CurrentNode.Child[1] = std::unique_ptr<KDNode>(new KDNode());

	// check for objects straddling the dividing axis, if so, add them to array
	for (int i = DividingObjectIndex; i >= 0; i--)
	{
		const AABB& CurrentBBox = CurrentObjects[i]->GetWorldAABB();
		const float AxisRange = CurrentBBox.GetDeminsions()[DividingAxis];
		const float DividedRange = std::abs(CurrentBBox.Min[DividingAxis] - DividingAxisValue);
		if (AxisRange > DividedRange)
			StraddlingPrimitives.push_back(CurrentObjects[i]);
		else
		{
			// add non straddling to near child
			CurrentNode.Child[0]->ObjectList.push_back(CurrentObjects[i]);
		}
	}

	for (size_t i = DividingObjectIndex + 1; i < PrimitiveListSize; i++)
	{
		const AABB& CurrentBBox = CurrentObjects[i]->GetWorldAABB();
		float AxisRange = CurrentBBox.GetDeminsions()[DividingAxis];
		float DividedRange = CurrentBBox.Max[DividingAxis] - DividingAxisValue;
		if (AxisRange > DividedRange)
			StraddlingPrimitives.push_back(CurrentObjects[i]);
		else
		{
			// add non straddling to far child
			CurrentNode.Child[1]->ObjectList.push_back(CurrentObjects[i]);
		}
	}

	CurrentNode.Child[0]->ObjectList.shrink_to_fit();
	CurrentNode.Child[1]->ObjectList.shrink_to_fit();

	// increment axis for children
	CurrentNode.Child[0]->Axis = CurrentNode.Child[1]->Axis = (DividingAxis + 1) % 3;

	// add all straddling objects to this node
	CurrentNode.ObjectList.clear();
	CurrentNode.ObjectList = StraddlingPrimitives;

	BuildTreeHelper(*CurrentNode.Child[0], Depth - 1, MinObjectsPerNode);
	BuildTreeHelper(*CurrentNode.Child[1], Depth - 1, MinObjectsPerNode);
}

bool KDTree::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	return VisitNodesAgainstRay(&mRoot, Ray, tValueOut, IntersectionOut);
}

bool KDTree::VisitNodesAgainstRay(KDNode* CurrentNode, FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	if (CurrentNode == nullptr)
		return false;

	bool IsIntersecting = false;

	for (IDrawable* primitive : CurrentNode->ObjectList)
	{
		IsIntersecting |= primitive->IsIntersectingRay(Ray, tValueOut, IntersectionOut);
	}

	// if no IntersectionOut, return when a valid intersection is hit
	if (!IntersectionOut && IsIntersecting)
		return true;

	// check which child to traverse first from axis split
	uint32_t Axis = CurrentNode->Axis;
	uint32_t FirstChild = Ray.origin[Axis] > CurrentNode->SplitValue;

	if (abs(Ray.direction[Axis]) < _EPSILON)
	{
		// Ray is parallel to the plane, visit only near side
		IsIntersecting |= VisitNodesAgainstRay(CurrentNode->Child[FirstChild].get(), Ray, tValueOut, IntersectionOut);
	}
	else
	{
		// Find t value of intersection of ray with split plane
		float t = (CurrentNode->SplitValue - Ray.origin[Axis]) / Ray.direction[Axis];

		// Check if ray straddles the splitting plane
		// If no tValueout is given, make one
		float maxTValue = (tValueOut) ? *tValueOut : std::numeric_limits<float>::max();

		if (0.0f <= t && t < maxTValue)
		{
			// Check for intersection in the near field, then far
			IsIntersecting |= VisitNodesAgainstRay(CurrentNode->Child[FirstChild].get(), Ray, tValueOut, IntersectionOut);
			IsIntersecting |= VisitNodesAgainstRay(CurrentNode->Child[FirstChild ^ 1].get(), Ray, tValueOut, IntersectionOut);
		}
		else
		{
			// Just check near side
			IsIntersecting |= VisitNodesAgainstRay(CurrentNode->Child[FirstChild].get(), Ray, tValueOut, IntersectionOut);
		}
	}

	return IsIntersecting;
}