#include "Mesh.h"
#include "Intersection.h"

#include <iostream>
#include <fstream>
#include <limits>

FMesh::FMesh(const FMaterial& Material)
	: IDrawable(Material)
	, mTriangles()
{
}

FMesh::FMesh(const std::string& ModelFilepath, const FMaterial& Material)
	: IDrawable(Material)
	, mTriangles()
{
	ReadModel(ModelFilepath);
}

FMesh::FMesh(const std::string& ModelFilepath, FTexture& Texture)
	: IDrawable(FMaterial())
	, mTriangles()
{
	mDiffuseTexture = &Texture;
	ReadModel(ModelFilepath);
}

bool FMesh::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	// check against bounding volume first
	float NewTValue = (tValueOut) ? *tValueOut : std::numeric_limits<float>::max();

	// bring ray into object space for intersection tests
	Ray = GetWorldInvTransform().TransformRay(Ray);

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
				IntersectionOut->point = GetWorldTransform().TransformPosition(IntersectionOut->point);
				IntersectionOut->normal = GetWorldTransform().TransformDirection(IntersectionOut->normal);
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

FMaterial FMesh::GetMaterial(Vector3f SurfacePoint)
{
	// remove compiler warning
	SurfacePoint;
	return mMaterial;
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
			Triangle->SetTexture(mDiffuseTexture);
			Triangle->SetParent(*this);

			if (HasUVs)
				Triangle->SetUVCoordinates(FaceUVs[2], FaceUVs[1], FaceUVs[0]);

			mTriangles.push_back(std::move(Triangle));
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
}

FMesh::~FMesh()
{
}
