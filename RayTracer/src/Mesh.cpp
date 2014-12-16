#include "Mesh.h"
#include "Intersection.h"

#include <iostream>
#include <fstream>
#include <limits>

FMesh::FMesh(const FMaterial& Material)
	: IDrawable()
	, mTriangles()
{
}

FMesh::FMesh(const std::string& ModelFilepath, const FMaterial& Material)
	: IDrawable(FMaterial())
	, mTriangles()
{
	ReadModel(ModelFilepath, Material);
}


bool FMesh::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	// check against bounding volume first
	float NewTValue = (tValueOut) ? *tValueOut : std::numeric_limits<float>::max();

	// bring ray into object space for intersection tests
	Ray = Transform.GetInverse().TransformRay(Ray);

	if (GetBoundingBox().IsIntersectingRay(Ray, &NewTValue, IntersectionOut))
	{
		// if we have a tValueOut then we are looking for the closest intersection
		// so we need to find the closest triangle
		if (tValueOut && NewTValue < *tValueOut)
		{
			NewTValue = (tValueOut) ? *tValueOut : std::numeric_limits<float>::max();
			bool Flag = false;
			for (const auto& Triangle : mTriangles)
			{
				Flag |= Triangle->IsIntersectingRay(Ray, &NewTValue, IntersectionOut);
			}

			if (Flag && NewTValue < *tValueOut)
			{
				*tValueOut = NewTValue;
				IntersectionOut->point = Transform.TransformPosition(IntersectionOut->point);
				IntersectionOut->normal = Transform.TransformDirection(IntersectionOut->normal);
			}

			return Flag;
		}
		// if no tValueOut, then we are just checking for a single intersection
		else
		{
			for (const auto& Triangle : mTriangles)
			{
				if (Triangle->IsIntersectingRay(Ray))
					return true;
			}
		}
	}

	return false;
}

void FMesh::SetMaterial(const FMaterial& Material)
{
	for (auto& Triangle : mTriangles)
		Triangle->SetMaterial(Material);
}

void FMesh::ConstructAABB(Vector3f Min, Vector3f Max)
{
	SetBoundingBox(AABB(Min, Max));
}

void FMesh::ReadModel(const std::string& ModelFilepath, const FMaterial& Material)
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

			const std::string V0String = FileLine.substr(0, FileLine.find(' '));
			const Vector3f V0 = Vertices[atoi(V0String.c_str()) - 1];

			FileLine = FileLine.substr(V0String.length() + 1);
			const std::string V1String = FileLine.substr(0, FileLine.find(' '));
			const Vector3f V1 = Vertices[atoi(V1String.c_str()) - 1];

			FileLine = FileLine.substr(V1String.length() + 1);
			const std::string V2String = FileLine.substr(0, FileLine.find(' '));
			const Vector3f V2 = Vertices[atoi(V2String.c_str()) - 1];

			// .obj vertex order is clockwise, we use counterclockwise
			mTriangles.push_back(std::unique_ptr<FTriangle>(new FTriangle(V2, V1, V0, Material)));
		}
	}

	ModelFile.close();

	ConstructAABB(MinBounds, MaxBounds);
}

FMesh::~FMesh()
{
}
