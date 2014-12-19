#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Ray.h"
#include "Cube.h"
#include "Mesh.h"
#include "Texture.h"
#include "FMath.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <limits>

static const Vector2i OutputResolution(1000, 600);
static std::vector<FTexture> TextureHolder;

//////////////////////////////////////////////////////////////////////////////////////////////

void throwSceneConfigError(const std::string& ObjectType)
{
	std::cout << "...Error in scene config file for a " << ObjectType << std::endl;
	throw std::runtime_error("Scene config error.");
}

//////////////////////////////////////////////////////////////////////////////////////////////
FScene::FScene()
	: mOutputImage("RenderedScene", OutputResolution)
	, mBackgroundColor(FColor::White)
	, mGlobalAmbient(0.2f, 0.2f, 0.2f)
	, mCamera(Vector3f(0, 10, 0), Vector3f(0, 0, -15.0f), Vector3f(0, 1, 0), 75, OutputResolution)
	, mPrimitives()
	, mLights()
	, mKDTree()
	, mNumberOfShadowSamples(1)
	, mSuperSamplingLevel(1)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////

void FScene::BuildScene(std::istream& in)
{
	std::string string;
	in >> string;

	while (in.good())
	{
		//if (string == "BackgroundColor:")
			//in >> mBackgroundColor.r >> mBackgroundColor.g >> mBackgroundColor.b;
		if (string == "GlobalAmbientColor:")
			in >> mGlobalAmbient.r >> mGlobalAmbient.g >> mGlobalAmbient.b;
		else if(string == "Camera")
		{
			Vector3f position;
			float FOV;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Camera");
			in >> position.x >> position.y >> position.z;

			in >> string;
			if (string != "FOV:")
				throwSceneConfigError("Camera");
			in >> FOV;

			//mCamera.SetPosition(position);
			mCamera.SetFOV(FOV);
		}
		else if (string == "DirectionalLight")
		{
			FColor color;
			Vector3f direction;

			in >> string;
			if (string != "Color:")
				throwSceneConfigError("DirectionalLight");
			in >> color.r >> color.g >> color.b;

			in >> string;
			if (string != "Direction:")
				throwSceneConfigError("DirectionalLight");
			in >> direction.x >> direction.y >> direction.z;

			mLights.push_back(LightPtr(new FDirectionalLight(color, direction)));
		}
		else if (string == "PointLight")
		{
			FColor color;
			Vector3f position;

			in >> string;
			if (string != "Color:")
				throwSceneConfigError("PointLight");
			in >> color.r >> color.g >> color.b;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("PointLight");
			in >> position.x >> position.y >> position.z;

			mLights.push_back(LightPtr(new FPointLight(color, position, 4, 10, 35))); ///////////////////////////////////////CHANGE TO FILE INPUT
		}
		else if (string == "Plane")
		{
			Vector3f direction;
			Vector3f point;

			in >> string;
			if (string != "Direction:")
				throwSceneConfigError("Plane");
			in >> direction.x >> direction.y >> direction.z;
			in >> string;
			if (string != "Point:")
				throwSceneConfigError("Plane");
			in >> point.x >> point.y >> point.z;

			FMaterial material(ReadMaterial(in));

			mPrimitives.push_back(PrimitivePtr(new FPlane(material, direction, point)));
		}
		else if (string == "Sphere")
		{
			Vector3f center;
			float radius;

			in >> string;
			if (string != "Center:")
				throwSceneConfigError("Sphere");
			in >> center.x >> center.y >> center.z;
			in >> string;
			if (string != "Radius:")
				throwSceneConfigError("Sphere");
			in >> radius;
			
			FMaterial material(ReadMaterial(in));

			mPrimitives.push_back(PrimitivePtr(new FSphere(center, radius, material)));
		}
		else if (string == "Model")
		{
			std::string filename;
			Vector3f translation;

			in >> string;
			if (string != "File:")
				throwSceneConfigError("Model");
			in >> filename;

			in >> string;
			if (string != "Translation:")
				throwSceneConfigError("Model");
			in >> translation.x >> translation.y >> translation.z;

			FMaterial material(ReadMaterial(in));

			ReadModel(filename, translation, material);
		}
		in >> string;
	}

	TextureHolder.push_back(FTexture("Textures/Danbotexture.tga"));
	TextureHolder.push_back(FTexture("Textures/Bricks.tga"));
	TextureHolder.push_back(FTexture("Textures/RockDiff.tga"));
	TextureHolder.push_back(FTexture("Textures/Checker.tga"));

	FTextureInfo BrickPlaneInfo{ &TextureHolder[1], 0.05f, 0.05f };
	FMaterial BrickPlaneMaterial(FColor(.3f, .1f, .1f), FColor(.8f, .2f, .2f), FColor(.1f, .1f, .1f), 1, 0.0f);
	BrickPlaneMaterial.SetDiffuseTexture(BrickPlaneInfo);

	FTextureInfo TilePlaneInfo{ &TextureHolder[3], 0.25f, 0.25f };
	FMaterial TilePlaneMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 128, 0.8f);
	TilePlaneMaterial.SetDiffuseTexture(TilePlaneInfo);

	FTextureInfo BrickBoxInfo{ &TextureHolder[1], 0.35f, 0.25f };
	FMaterial BrickBoxMaterial(FColor(.8f, .8f, .8f), FColor(.0f, .0f, .0f), FColor(.1f, .1f, .1f), 64, 0.0f);
	BrickBoxMaterial.SetDiffuseTexture(BrickBoxInfo);

	FTextureInfo BrickSphereInfo{ &TextureHolder[2], 0.5f, 0.5f };
	FMaterial BrickSphereMaterial(FColor(.8f, .8f, .8f), FColor(.0f, .0f, .0f), FColor(.1f, .1f, .1f), 64, 0.0f);
	BrickSphereMaterial.SetDiffuseTexture(BrickSphereInfo);

	FTextureInfo BoxManTextInfo{ &TextureHolder[0], 1.0f, 1.0f };
	FMaterial BoxManMaterial(FMaterial(FColor(.1f, .1f, .1f), FColor(.1f, .1f, .1f), FColor(.1f, .1f, .1f), 12, 0.0f));
	BoxManMaterial.SetDiffuseTexture(BoxManTextInfo);
	
	//mLights.push_back(LightPtr(new FPointLight(FColor(0.6f, 0.1f, 0.4f), Vector3f(3.0f, 10.0f, -20.0f), 2, 2, 20)));
	//mLights.push_back(LightPtr(new FPointLight(FColor(0.3f, 1.0f, 1.0f), Vector3f(2.0f, 7, -10.0f), 1, 10, 35)));
	//mLights.push_back(LightPtr(new FDirectionalLight(FColor(0.6f, 0.6f, 0.6f), Vector3f(-1.0f, -1.0f, -2.0f))));

	IDrawable* Drawable;
	mPrimitives.push_back(PrimitivePtr(new FPlane(TilePlaneMaterial, Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0, -5.0f, 0))));
	//mPrimitives.push_back(PrimitivePtr(new FPlane(FMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 64, 0.2f), Vector3f(0.0f, -1.0f, 0.0f), Vector3f(0, 10.0f, 0))));
	//mPrimitives.push_back(PrimitivePtr(new FPlane(FMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 64, 0.2f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(-15.0f, 0.0f, 0))));
	//mPrimitives.push_back(PrimitivePtr(new FPlane(FMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 64, 0.2f), Vector3f(-1.0f, 0.0f, 0.0f), Vector3f(15.0f, 0.0f, 0))));
	//mPrimitives.push_back(PrimitivePtr(new FPlane(FMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 64, 0.2f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0, 0.0f, -35.0f))));
	
	//std::unique_ptr<FTriangle> Triangle(new FTriangle(Vector3f(-15.0f, 7.0f, -18.0f), Vector3f(15.0f, 8.0f, -18.0f), Vector3f(-15.0f, -11.0f, -18.0f)));
	//Triangle->SetTexture(&TextureHolder[0]);
	//Triangle->SetMaterial(FMaterial(FColor(0.0f, .0f, .0f), FColor(.1f, .1f, .1f), FColor(.1f, .1f, .1f), 12, 0.1f));
	//Triangle->SetUVCoordinates(Vector2f(0.0f, 0.0f), Vector2f(1.0f, 0.0f), Vector2f(0.0f, 1.0f));
	//mPrimitives.push_back(std::move(Triangle));

	mLights.push_back(LightPtr(new FPointLight(FColor::White, Vector3f(-0.0f, 14.0f, -10.0f), 1.5f, 10, 35)));
	//mPrimitives.push_back(PrimitivePtr(new FMesh("Models/BoxMan.obj", BoxManMaterial)));
	//FMatrix4 trans;
	//trans.SetOrigin(Vector3f(1.0f, -2.0f, -24));
	//trans.Rotate(EAxis::Y, -25);
	//Drawable->SetTransform(trans);
	///*mPrimitives.back().get()->Transform.SetOrigin(Vector3f(5, -1.0f, -15));
	//mPrimitives.back().get()->Transform.Rotate(FMatrix4::Axis::Y, -25);*/

	//mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(-0.0f, -1.5f, -15.0f), FMaterial(FColor(.9f, .1f, .1f), FColor(.9f, .1f, .1f), FColor(.1f, .1f, .1f), 32, .1f))));
	//Drawable = mPrimitives.back().get();
	//Drawable->Transform.Rotate(FMatrix4::Axis::X, 25);
	//Drawable->Transform.Scale(Vector3f(3,.5,2));
	
	//mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(-5.0f, -2.0f, -15.0f), FMaterial(FColor(.2f, .7f, .7f), FColor(.5f, 1.0f, .5f), FColor(.1f, .1f, .1f), 64, .9f))));
	//cube = mPrimitives.back().get();
	//cube->Transform.Scale(0.5f);

	//mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(3.0f, -3.5f, -13.0f), FMaterial(FColor(.2f, .7f, .7f), FColor(.1f, .3f, .8f, 0.7f), FColor(.1f, .1f, .1f), 64, .0f))));
	//Drawable = mPrimitives.back().get();
	//Drawable->Scale(EAxis::Y, 1.5f);
	//Drawable->Scale(EAxis::Z, 1.5f);
	//Drawable->Scale(EAxis::X, 1.5f);

	//mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(-5.0f, -2.0f, -15.0f), BrickBoxMaterial)));
	//Drawable = mPrimitives.back().get();
	//Drawable->Scale(EAxis::Y, 1.5f);
	//Drawable->Scale(EAxis::Z, 3.25f);
	//Drawable->Scale(EAxis::X, 0.25f);
	//Drawable->Rotate(EAxis::Y, -10.0f);

	//mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(-4.0f, 1.0f, -12.5f), 1.0f, FMaterial(FColor(.2f, .7f, .7f), FColor(1.0f, .4f, .1f), FColor(.1f, .1f, .1f), 64, .9f))));

	//mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(5.0f, 2.0f, -12.0f), 2.0f, BrickSphereMaterial)));

	//mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(-5.0f, 1.0f, -15.0f), 2.0f, FMaterial(FColor(.8f, .3f, .1f), FColor(0.7f, .3f, .1f, 0.4f), FColor(.1f, .1f, .1f), 128, 0.0f, 1.4f))));
	//mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(-1.0f, 0.0f, -15.0f), 3.0f, FMaterial(FColor(0.628f, 0.555f, 0.366f), FColor(0.7516f, 0.606f, 0.226f, 1.0f), FColor(.24725f, .1995f, .0745f), 102.4f, 0.7f, 0.0f))));
	mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(5.0f, 1.0f, -15.0f), 2.0f, FMaterial(FColor(.8f, .3f, .1f), FColor(0.7f, .3f, .1f, 0.4f), FColor(.1f, .1f, .1f), 128, 0.0f, 1.33f))));

	//mKDTree.buildTree(mPrimitives, 10);
}

//////////////////////////////////////////////////////////////////////////////////////////////

FColor FScene::TraceRay(const FRay& CameraRay, int32_t Depth)
{
	if (Depth < 1)
		return mBackgroundColor;

	float MaxTValue(std::numeric_limits<float>::max());
	FIntersection ClosestIntersection;

	
	 //For performs difference tests
	for (const auto& Primitive : mPrimitives)
	{
		Primitive->IsIntersectingRay(CameraRay, &MaxTValue, &ClosestIntersection);
	}
		
	//mKDTree.IsIntersectingRay(cameraRay, &maxTValue, &closestIntersection);

	// If an object was intersected
	if (ClosestIntersection.object)
	{
		// disable the current object so reflection and refraction rays don't
		// interact with it
		ClosestIntersection.object->SetEnabled(false);
		FColor OutputColor;

		// Get the surface material, point, and normal
		const Vector3f& SurfacePoint(ClosestIntersection.point);
		Vector3f SurfaceNormal(ClosestIntersection.normal);
		const FMaterial& SurfaceMaterial(ClosestIntersection.object->GetMaterial(SurfacePoint));

		for (const auto& light : mLights)
		{
			FColor LightColor = light->GetIntesityAt(SurfacePoint);

			// if the light intesity is 0, skip this light
			if (LightColor == FColor::Black)
				continue;

			// Get direction of light and compute h reflection
			FRay RayToLight(light->GetRayToLight(SurfacePoint));
			RayToLight.origin += RayToLight.direction * _EPSILON;

			const Vector3f& LightDirection(RayToLight.direction);
			const Vector3f& H = ComputeBlinnSpecularReflection(RayToLight.direction, -CameraRay.direction);

			// If an object is in the way of the light, skip lighting for that light
			if (mNumberOfShadowSamples > 1)
			{
				const float ShadeFactor = ComputeShadeFactor(*light, SurfacePoint);
				if (ShadeFactor <= 0.0)
					continue;

				LightColor *= ShadeFactor;
			}
			else if (IsInShadow(RayToLight))
			{
				continue;
			}

			// Get dot product of surface normal and h for specular lighting
			float SpecularFactor = std::max(Vector3f::Dot(SurfaceNormal, H), 0.f);

			// Add glossiness expononent
			SpecularFactor = pow(SpecularFactor, SurfaceMaterial.GetGlossiness());

			// Get dot product of surface normal and light direction for diffuse lighting
			float DiffuseFactor = std::max(Vector3f::Dot(SurfaceNormal, LightDirection), 0.f);

			// Combine material color and light color for diffuse and specular
			FColor specularColor(LightColor * SurfaceMaterial.GetSpecular() * SpecularFactor);
			FColor diffuseColor(LightColor  * SurfaceMaterial.GetDiffuse() * DiffuseFactor);

			// Add diffuse and specular contributions to total
			OutputColor += specularColor + diffuseColor;

			if (SurfaceMaterial.GetDiffuse().a < 1.0f)
			{
				OutputColor *= SurfaceMaterial.GetDiffuse().a;
				const FRay Refraction(SurfacePoint, ComputeRefractionVector(-CameraRay.direction, SurfaceNormal, SurfaceMaterial.GetRefractiveIndex()));
				OutputColor += (1 - SurfaceMaterial.GetDiffuse().a) * TraceRay(Refraction, Depth - 1);
			}

			// Add mirror reflection contributions
			Vector3f mirrorReflection = -CameraRay.direction.Reflect(SurfaceNormal);
			FRay reflectionRay(SurfacePoint, mirrorReflection);
			OutputColor += TraceRay(reflectionRay, Depth - 1) * OutputColor * SurfaceMaterial.GetReflectivity();
			
		}

		// re-enable this object
		ClosestIntersection.object->SetEnabled(true);

		// return computed color totals with ambient contribution
		return  OutputColor + (mGlobalAmbient * SurfaceMaterial.GetAmbient());
	}
	else
		return mBackgroundColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void FScene::RenderScene()
{
	const float InvTotalPixels = 100.0f / (OutputResolution.x * OutputResolution.y);
	const int PercentUpdateRate = OutputResolution.y / 20;
	int UpdateInterval = 0;

	for (int y = 0; y < OutputResolution.y; y++)
	{
		for (int x = 0; x < OutputResolution.x; x++)
		{
			FColor PixelColor;

			if (mSuperSamplingLevel > 1)
			{
				for (const FRay& PixelRay : mCamera.GenerateSampleRays(x, y, mSuperSamplingLevel))
				{
					PixelColor += TraceRay(PixelRay, 5);
				}

				PixelColor /= (float)(mSuperSamplingLevel * mSuperSamplingLevel); // average the result of all samples
			}
			else
			{
				const FRay& PixelRay = mCamera.GenerateRay(x, y);
				PixelColor = TraceRay(PixelRay, 5);
			}

			mOutputImage.setPixel(x, y, PixelColor.Clamp());
		}

		// Display progress to console
		UpdateInterval++;

		if (UpdateInterval >= PercentUpdateRate)
		{
			UpdateInterval = 0;
			std::cout << (y * OutputResolution.x) * InvTotalPixels << "% Complete" << std::endl;
		}
	}

	mOutputImage.writeImage();
}

//////////////////////////////////////////////////////////////////////////////////////////////

Vector3f FScene::ComputeBlinnSpecularReflection(const Vector3f& LightDirection, const Vector3f& ViewerDirection) const
{
	// use half-way vector
	Vector3f Reflection(LightDirection + ViewerDirection);
	Reflection.Normalize();
	return Reflection;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Vector3f FScene::ComputeRefractionVector(const Vector3f& LightDirection, const Vector3f& SurfaceNormal, const float& RefractiveIndex) const
{
	const float& InvRefractive = 1.0f / RefractiveIndex;
	const float& NDotL = Vector3f::Dot(SurfaceNormal, LightDirection);

	return ((InvRefractive * NDotL - std::sqrtf(1 - InvRefractive * InvRefractive * (1 - (NDotL * NDotL)))) * SurfaceNormal - InvRefractive * LightDirection).Normalize();
}

bool FScene::IsInShadow(const FRay& LightRay)
{
	//return mKDTree.IsIntersectingRay(LightRay);

	// For performance tests
	for (const auto& Primitive : mPrimitives)
	{
		// If the intersects the light
		if (Primitive->IsIntersectingRay(LightRay))
			return true;
	}

	return false;
	
}

float FScene::ComputeShadeFactor(const ILight& Light, const Vector3f& SurfacePoint) const
{
	const float FactorSize = 1.0f / mNumberOfShadowSamples;
	float ShadeFactor = 1.0f;

	for (FRay ShadowSample : Light.GetRayToLightSamples(SurfacePoint, mNumberOfShadowSamples))
	{
		// make sure the ray doesn't start below the surface
		ShadowSample.origin += ShadowSample.direction * _EPSILON;
		for (const auto& Primitive : mPrimitives)
		{
			// If the object is not the reference one and intersects the light
			if (Primitive->IsIntersectingRay(ShadowSample))
			{
				ShadeFactor -= FactorSize * Primitive->GetMaterial().GetDiffuse().a;
				break;
			}
		}
	}

	return ShadeFactor;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Vector3f FScene::ComputeMirriorReflection(const Vector3f& ViewerDirection, const Vector3f& SurfaceNormal) const
{
	float ViewerDotNormal = Vector3f::Dot(SurfaceNormal, ViewerDirection);
	Vector3f ReflectionDirection(2 * (ViewerDotNormal)* SurfaceNormal - ViewerDirection);
	ReflectionDirection.Normalize();
	return ReflectionDirection;
}

//////////////////////////////////////////////////////////////////////////////////////////////

FMaterial FScene::ReadMaterial(std::istream& input)
{
	FColor specular, diffuse, ambient;
	float specualarExponent, reflectivity;

	std::string materialProperty;
	
	input >> materialProperty;
	if (materialProperty != "Specular:")
		throwSceneConfigError("Material");
	input >> specular.r >> specular.g >> specular.b;

	input >> materialProperty;
	if (materialProperty != "Diffuse:")
		throwSceneConfigError("Material");
	input >> diffuse.r >> diffuse.g >> diffuse.b;

	input >> materialProperty;
	if (materialProperty != "Ambient:")
		throwSceneConfigError("Material");
	input >> ambient.r >> ambient.g >> ambient.b;

	input >> materialProperty;
	if (materialProperty != "SpecularExponent:")
		throwSceneConfigError("Material");
	input >> specualarExponent;

	input >> materialProperty;
	if (materialProperty != "Reflectivity:")
		throwSceneConfigError("Material");
	input >> reflectivity;

	return FMaterial(specular, diffuse, ambient, specualarExponent, reflectivity);
}

void FScene::ReadModel(std::string filename, Vector3f translation, FMaterial material)
{
	std::filebuf modelBuffer;
	if (modelBuffer.open(filename, std::ios::in))
	{
		std::istream in(&modelBuffer);

		std::string string;
		std::size_t vertexIndex;
		Vector3f vertex;
		std::vector<Vector3f> vertices;

		// Vertex indices for each face
		std::size_t faceV0, faceV1, faceV2;

		in >> string;

		while (in.good())
		{
			if (string == "Vertex")
			{
				in >> vertexIndex;
				in >> vertex.x >> vertex.y >> vertex.z;
				vertex *= 2;
				vertex += translation;
				vertices.push_back(vertex);
			}
			else if (string == "Face")
			{
				// pull out face number
				in >> string;

				// Get vertices' indices
				in >> faceV0;
				in >> faceV1;
				in >> faceV2;

				mPrimitives.push_back(PrimitivePtr(new FTriangle(vertices[faceV0-1], vertices[faceV1-1], vertices[faceV2-1], material)));
			}

			in >> string;
		}
	}

	modelBuffer.close();
}