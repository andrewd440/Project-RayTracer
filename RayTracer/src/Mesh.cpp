#include "Mesh.h"

#include <iostream>
#include <fstream>

FMesh::FMesh()
	: IDrawable()
	, mTriangles()
{
}

FMesh::FMesh(const std::string& ModelFilepath)
	: IDrawable(FMaterial())
	, mTriangles()
{
	ReadModel(ModelFilepath);
}


bool FMesh::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	if (tValueOut)
	{
		bool Flag;
		for (FTriangle Triangle : mTriangles)
		{
			Flag |= Triangle.IsIntersectingRay(Ray, tValueOut, IntersectionOut);
		}

		return Flag;
	}
	else
	{
		for (FTriangle Triangle : mTriangles)
		{
			if (Triangle.IsIntersectingRay(Ray))
				return true;
		}
	}

	return false;
}

void FMesh::ConstructAABB()
{

}

void FMesh::ReadModel(const std::string& ModelFilepath)
{
	std::ifstream ModelFile(ModelFilepath.c_str());

	if (ModelFile.fail())
	{
		std::cout << "Failed to open FMesh model filename: "
			<< ModelFilepath;
		return;
	}

	std::string FileLine;
	std::vector<Vector3f> vertices;

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

			vertices.push_back(Vector3f(X, Y, Z));
			
		}
		// line contains a face
		else if (FileLine[0] == 'f' && FileLine[1] == ' ')
		{
			FileLine = FileLine.substr(2);

			const std::string V0String = FileLine.substr(0, FileLine.find(' '));
			const Vector3f V0 = vertices[atoi(V0String.c_str()) - 1];

			FileLine = FileLine.substr(V0String.length() + 1);
			const std::string V1String = FileLine.substr(0, FileLine.find(' '));
			const Vector3f V1 = vertices[atoi(V1String.c_str()) - 1];

			FileLine = FileLine.substr(V1String.length() + 1);
			const std::string V2String = FileLine.substr(0, FileLine.find(' '));
			const Vector3f V2 = vertices[atoi(V2String.c_str()) - 1];

			mTriangles.push_back(FTriangle(V0, V1, V2));
		}
	}

	ModelFile.close();
}

FMesh::~FMesh()
{
}
