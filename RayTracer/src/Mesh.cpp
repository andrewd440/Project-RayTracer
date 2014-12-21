#include "Mesh.h"
#include "Intersection.h"

#include <iostream>
#include <fstream>
#include <limits>

#define _BVH_MIN_OBJECTS 5

FMesh::FMesh(const FMaterial& Material)
	: IDrawable(Material)
	, mBVHRoot()
{
}

FMesh::FMesh(const std::string& ModelFilepath, const FMaterial& Material)
	: IDrawable(Material)
	, mBVHRoot()
{
	ReadModel(ModelFilepath);
	ConstructBVH(mBVHRoot, 8);
}

bool FMesh::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	if (!IsEnabled())
		return false;

	// bring ray into object space for intersection tests
	Ray = GetWorldInvTransform().TransformRay(Ray);

	const bool Flag = TraverseBVHAgainstRay(mBVHRoot, Ray, tValueOut, IntersectionOut);

	if (Flag && tValueOut && IntersectionOut)
	{
		const FMatrix4& WorldTransform = GetWorldTransform();
		IntersectionOut->point = WorldTransform.TransformPosition(IntersectionOut->point);
		IntersectionOut->normal = WorldTransform.TransformDirection(IntersectionOut->normal);
	}

	return Flag;
}

bool FMesh::TraverseBVHAgainstRay(FBVHNode& Node, FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	float TempTValue = (tValueOut) ? *tValueOut : std::numeric_limits<float>::max();
	if (!Node.BoundingVolume.IsIntersectingRay(Ray, &TempTValue))
		return false;

	if (!Node.Objects.empty())
	{
		bool IsIntersecting = false;
		for (const auto& Object : Node.Objects)
		{
			IsIntersecting |= Object->IsIntersectingRay(Ray, tValueOut, IntersectionOut);
		}

		return IsIntersecting;
	}

	bool IsIntersecting = false;
	IsIntersecting |= TraverseBVHAgainstRay(*Node.Child[1], Ray, tValueOut, IntersectionOut);
	IsIntersecting |= TraverseBVHAgainstRay(*Node.Child[0], Ray, tValueOut, IntersectionOut);

	return IsIntersecting;

}

FMaterial FMesh::GetMaterial(Vector3f SurfacePoint)
{
	// remove compiler warning
	SurfacePoint;
	return mMaterial;
}

void FMesh::ConstructAABB(Vector3f Min, Vector3f Max)
{
	SetBoundingBox(AABB(Min, Max));
}

void FMesh::ReadModel(const std::string& ModelFilepath)
{
	// bounds for bounding box
	Vector3f MinBounds;
	Vector3f MaxBounds;

	std::ifstream ModelFile(ModelFilepath.c_str());

	if (ModelFile.fail())
	{
		std::cout << "Failed to open FMesh model filename: "
			<< ModelFilepath;
		return;
	}

	std::string FileLine;
	std::vector<Vector3f> Vertices;
	std::vector<Vector2f> UVs;
	std::vector<std::unique_ptr<FTriangle>> Triangles;

	while (getline(ModelFile, FileLine))
	{
		// line contains a vertex
		if (FileLine[0] == 'v' && FileLine[1] == ' ')
		{
			FileLine = FileLine.substr(2);

			const std::string XString = FileLine.substr(0, FileLine.find(' '));
			const float X = (float)atof(XString.c_str());

			FileLine = FileLine.substr(XString.length() + 1);
			const std::string YString = FileLine.substr(0, FileLine.find(' '));
			const float Y = (float)atof(YString.c_str());

			FileLine = FileLine.substr(YString.length() + 1);
			const std::string ZString = FileLine.substr(0, FileLine.find(' '));
			const float Z = (float)atof(ZString.c_str());

			const Vector3f Vertex(X, Y, Z);
			UpdateBounds(MinBounds, MaxBounds, Vertex);

			Vertices.push_back(Vertex);
			
		}
		// line contains a face
		else if (FileLine[0] == 'f' && FileLine[1] == ' ')
		{
			FileLine = FileLine.substr(2);

			Vector3f FaceVerts[3];

			bool HasUVs = false;
			Vector2f FaceUVs[3];

			// get attributes for each vertex
			for (int i = 0; i < 3; i++)
			{
				std::string DataString = FileLine.substr(0, FileLine.find(' '));
				std::string VertexString = DataString.substr(0, DataString.find('/'));
				FaceVerts[i] = Vertices[atoi(VertexString.c_str()) - 1];

				if (HasUVs || VertexString.length() != DataString.length())
				{
					HasUVs = true;
					std::string UVString = DataString.substr(VertexString.length() + 1);
					UVString = UVString.substr(0, UVString.find('/'));
					FaceUVs[i] = UVs[atoi(UVString.c_str()) - 1];
				}

				if (i < 2)
					FileLine = FileLine.substr(DataString.length() + 1);
			}
			
			// .obj vertex order is clockwise, we use counterclockwise
			std::unique_ptr<FTriangle> Triangle(new FTriangle(FaceVerts[2], FaceVerts[1], FaceVerts[0], mMaterial));
			Triangle->SetParent(*this);

			if (HasUVs)
				Triangle->SetUVCoordinates(FaceUVs[2], FaceUVs[1], FaceUVs[0]);

			Triangles.push_back(std::move(Triangle));
		}
		// line contains a UV
		else if (FileLine[0] == 'v' && FileLine[1] == 't' && FileLine[2] == ' ')
		{
			FileLine = FileLine.substr(3);

			const std::string UString = FileLine.substr(0, FileLine.find(' '));
			const std::string VString = FileLine.substr(UString.length() + 1, FileLine.find(' '));
			UVs.push_back(Vector2f((float)atof(UString.c_str()), (float)atof(VString.c_str())));
		}
	}

	ModelFile.close();

	ConstructAABB(MinBounds, MaxBounds);

	// setup root of BVH
	mBVHRoot.BoundingVolume = GetBoundingBox();
	mBVHRoot.Objects = std::move(Triangles);
}

void FMesh::ConstructBVH(FBVHNode& Node, const uint8_t Depth)
{
	if (Depth <= 0 || Node.Objects.size() <= _BVH_MIN_OBJECTS)
		return;

	// the split axis will be the axis which the current AABB extends the most
	uint8_t SplitAxis = 0;
	const Vector3f& CurrentDeminsions(Node.BoundingVolume.GetDeminsions());
	for (uint8_t i = 1; i < 3; i++)
	{
		if (CurrentDeminsions[i] > CurrentDeminsions[SplitAxis])
			SplitAxis = i;
	}

	// sort objects in respect to the split axis
	std::sort(Node.Objects.begin(), Node.Objects.end(), [&SplitAxis](const std::unique_ptr<FTriangle>& Lhs, const std::unique_ptr<FTriangle>& Rhs)
	{
		return Lhs->GetBoundingBox().GetCenter()[SplitAxis] < Rhs->GetBoundingBox().GetCenter()[SplitAxis];
	});
	
	// split at the median object
	const uint32_t SplitIndex = Node.Objects.size() / 2;

	// construct children and divide objects amoung them
	Node.Child[0] = std::unique_ptr<FBVHNode>(new FBVHNode());
	Node.Child[1] = std::unique_ptr<FBVHNode>(new FBVHNode());

	for (size_t i = 0; i < SplitIndex; i++)
	{
		Node.Child[0]->Objects.push_back(std::move(Node.Objects[i]));
	}

	for (size_t i = SplitIndex; i < Node.Objects.size(); i++)
	{
		Node.Child[1]->Objects.push_back(std::move(Node.Objects[i]));
	}

	Node.Objects.clear();
	Node.Objects.resize(0);

	// Construct bounding box for each child
	ConstructBoundingVolume(*Node.Child[0]);
	ConstructBoundingVolume(*Node.Child[1]);

	ConstructBVH(*Node.Child[0], Depth - 1);
	ConstructBVH(*Node.Child[1], Depth - 1);
}

void FMesh::ConstructBoundingVolume(FBVHNode& Node)
{
	Vector3f Min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vector3f Max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

	for (const auto& Triangle : Node.Objects)
	{
		const AABB& CurrentAABB = Triangle->GetBoundingBox();
		UpdateBounds(Min, Max, CurrentAABB.Min);
		UpdateBounds(Min, Max, CurrentAABB.Max);
	}

	Node.BoundingVolume.Min = Min;
	Node.BoundingVolume.Max = Max;
}

FMesh::~FMesh()
{
}
