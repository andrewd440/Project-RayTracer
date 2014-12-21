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
#include <unordered_map>

static std::unordered_map<std::string, FTexture> TextureHolder;
static std::unordered_map<std::string, FMaterial> MaterialHolder;
static const uint8_t KdDepth = 10;
static const uint8_t KdMinObjects = 3;

//////////////////////////////////////////////////////////////////////////////////////////////

void throwSceneConfigError(const std::string& ObjectType)
{
	std::cout << "...Error in scene config file for a " << ObjectType << std::endl;
	throw std::runtime_error("Scene config error.");
}

//////////////////////////////////////////////////////////////////////////////////////////////
FScene::FScene(const std::string& OutputName, const Vector2i& OutputResolution, const uint16_t NumShadowSamples, const uint16_t SuperSamplingLevel)
	: mOutputImage(OutputName, OutputResolution)
	, mBackgroundColor(FColor::Black)
	, mGlobalAmbient(0.2f, 0.2f, 0.2f)
	, mCamera(Vector3f(0, 0, 0), Vector3f(0, 0, -1.0f), Vector3f(0, 1, 0), 75, OutputResolution)
	, mLights()
	, mKDTree()
	, mNumberOfShadowSamples(NumShadowSamples)
	, mSuperSamplingLevel(SuperSamplingLevel)
	, mOutputResolution(OutputResolution)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////

void FScene::BuildScene(std::istream& in)
{
	std::vector<PrimitivePtr> Objects;

	std::string string;
	in >> string;

	while (in.good())
	{
		if (string == "Background:")
			in >> mBackgroundColor.R >> mBackgroundColor.G >> mBackgroundColor.B;
		else if (string == "GlobalAmbient:")
			in >> mGlobalAmbient.R >> mGlobalAmbient.G >> mGlobalAmbient.B;
		else if(string == "Camera")
		{
			Vector3f Position, LookAt, Up;
			float FOV;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Camera");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "LookAt:")
				throwSceneConfigError("Camera");
			in >> LookAt.x >> LookAt.y >> LookAt.z;

			in >> string;
			if (string != "UpDirection:")
				throwSceneConfigError("Camera");
			in >> Up.x >> Up.y >> Up.z;

			in >> string;
			if (string != "FOV:")
				throwSceneConfigError("Camera");
			in >> FOV;

			mCamera = FCamera(Position, LookAt, Up, FOV, mOutputResolution);
		}
		else if (string == "DirectionalLight")
		{
			FColor Color;
			Vector3f Direction;

			in >> string;
			if (string != "Color:")
				throwSceneConfigError("DirectionalLight");
			in >> Color.R >> Color.G >> Color.B;

			in >> string;
			if (string != "Direction:")
				throwSceneConfigError("DirectionalLight");
			in >> Direction.x >> Direction.y >> Direction.z;

			mLights.push_back(LightPtr(new FDirectionalLight(Color, Direction)));
		}
		else if (string == "PointLight")
		{
			FColor Color;
			Vector3f Position;
			float SurfaceRadius, MinFalloff, MaxFalloff;

			in >> string;
			if (string != "Color:")
				throwSceneConfigError("PointLight");
			in >> Color.R >> Color.G >> Color.B;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("PointLight");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "SurfaceRadius:")
				throwSceneConfigError("PointLight");
			in >> SurfaceRadius;

			in >> string;
			if (string != "MinFalloff:")
				throwSceneConfigError("PointLight");
			in >> MinFalloff;

			in >> string;
			if (string != "MaxFalloff:")
				throwSceneConfigError("PointLight");
			in >> MaxFalloff;

			mLights.push_back(LightPtr(new FPointLight(Color, Position, SurfaceRadius, MinFalloff, MaxFalloff)));
		}
		else if (string == "Plane")
		{
			Vector3f Normal;
			Vector3f Point;
			std::string Material;

			in >> string;
			if (string != "Normal:")
				throwSceneConfigError("Plane");
			in >> Normal.x >> Normal.y >> Normal.z;

			in >> string;
			if (string != "PointOnPlane:")
				throwSceneConfigError("Plane");
			in >> Point.x >> Point.y >> Point.z;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Plane");
			in >> Material;

			Objects.push_back(PrimitivePtr(new FPlane(MaterialHolder[Material], Normal, Point)));
		}
		else if (string == "Sphere")
		{
			Vector3f Position, Rotation;
			float Radius;
			std::string Material;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Sphere");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "Radius:")
				throwSceneConfigError("Sphere");
			in >> Radius;

			in >> string;
			if (string != "Rotation:")
				throwSceneConfigError("Cube");
			in >> Rotation.x >> Rotation.y >> Rotation.z;
			
			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Sphere");
			in >> Material;

			Objects.push_back(PrimitivePtr(new FSphere(Position, Radius, MaterialHolder[Material])));
			FMatrix4 Transform;
			Transform.SetOrigin(Position);
			Transform.Rotate(Rotation);
			Objects.back()->SetTransform(Transform);
		}
		else if (string == "Triangle")
		{
			Vector3f V0, V1, V2;
			std::string Material;

			in >> string;
			if (string != "V0:")
				throwSceneConfigError("Triangle");
			in >> V0.x >> V0.y >> V0.z;

			in >> string;
			if (string != "V1:")
				throwSceneConfigError("Triangle");
			in >> V1.x >> V1.y >> V1.z;

			in >> string;
			if (string != "V2:")
				throwSceneConfigError("Triangle");
			in >> V2.x >> V2.y >> V2.z;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Triangle");
			in >> Material;

			Objects.push_back(PrimitivePtr(new FTriangle(V0, V1, V2, MaterialHolder[Material])));
		}
		else if (string == "Cube")
		{
			Vector3f Position, Rotation, Scale;
			std::string Material;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Cube");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "Rotation:")
				throwSceneConfigError("Cube");
			in >> Rotation.x >> Rotation.y >> Rotation.z;

			in >> string;
			if (string != "Scale:")
				throwSceneConfigError("Cube");
			in >> Scale.x >> Scale.y >> Scale.z;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Cube");
			in >> Material;

			Objects.push_back(PrimitivePtr(new FCube(Position, MaterialHolder[Material])));
			FMatrix4 Transform;
			Transform.SetOrigin(Position);
			Transform.Rotate(Rotation);
			Transform.Scale(Scale);
			Objects.back()->SetTransform(Transform);
		}
		else if (string == "Model")
		{
			std::string Filename, Material;
			Vector3f Position, Rotation, Scale;

			in >> string;
			if (string != "ModelFile:")
				throwSceneConfigError("Model");
			in >> Filename;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Model");
			in >> Material;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Model");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "Rotation:")
				throwSceneConfigError("Model");
			in >> Rotation.x >> Rotation.y >> Rotation.z;

			in >> string;
			if (string != "Scale:")
				throwSceneConfigError("Model");
			in >> Scale.x >> Scale.y >> Scale.z;

			Objects.push_back(PrimitivePtr(new FMesh(Filename, MaterialHolder[Material])));
			FMatrix4 Transform;
			Transform.SetOrigin(Position);
			Transform.Rotate(Rotation);
			Transform.Scale(Scale);
			Objects.back()->SetTransform(Transform);
		}
		else if (string == "Texture")
		{
			std::string Name, File;

			in >> string;
			if (string != "Name:")
				throwSceneConfigError("Texture");
			in >> Name;

			in >> string;
			if (string != "File:")
				throwSceneConfigError("Texture");
			in >> File;

			TextureHolder.insert({ Name, FTexture(File) });
		}
		else if (string == "Material")
		{
			std::string Name;
			FColor Specular, Diffuse, Ambient;
			float Glossiness, Reflectivity, RefractiveIndex;
			
			std::string DiffuseTextureName; 
			Vector2f UVScale;

			in >> string;
			if (string != "Name:")
				throwSceneConfigError("Material");
			in >> Name;

			in >> string;
			if (string != "Specular:")
				throwSceneConfigError("Material");
			in >> Specular.R >> Specular.G >> Specular.B >> Specular.A;

			in >> string;
			if (string != "Diffuse:")
				throwSceneConfigError("Material");
			in >> Diffuse.R >> Diffuse.G >> Diffuse.B >> Diffuse.A;

			in >> string;
			if (string != "Ambient:")
				throwSceneConfigError("Material");
			in >> Ambient.R >> Ambient.G >> Ambient.B >> Ambient.A;

			in >> string;
			if (string != "Glossiness:")
				throwSceneConfigError("Material");
			in >> Glossiness;

			in >> string;
			if (string != "Reflectivity:")
				throwSceneConfigError("Material");
			in >> Reflectivity;

			in >> string;
			if (string != "RefractiveIndex:")
				throwSceneConfigError("Material");
			in >> RefractiveIndex;

			in >> string;
			if (string != "DiffuseTextureName:")
				throwSceneConfigError("Material");
			in >> DiffuseTextureName;

			in >> string;
			if (string != "UVScale:")
				throwSceneConfigError("Material");
			in >> UVScale.x >> UVScale.y;

			FMaterial Material(Specular, Diffuse, Ambient, Glossiness, Reflectivity, RefractiveIndex);

			// if a texture was given, assign it to the material
			if (TextureHolder.find(DiffuseTextureName) != TextureHolder.end())
			{
				Material.SetDiffuseTexture(FTextureInfo{ &TextureHolder.at(DiffuseTextureName), UVScale.x, UVScale.y });
			}

			MaterialHolder.insert({ Name, Material });
		}
		in >> string;
	}

	mKDTree.BuildTree(std::move(Objects), KdDepth, KdMinObjects);
}

//////////////////////////////////////////////////////////////////////////////////////////////

FColor FScene::TraceRay(const FRay& CameraRay, int32_t Depth)
{
	if (Depth < 1)
		return mBackgroundColor;

	float MaxTValue(std::numeric_limits<float>::max());
	FIntersection ClosestIntersection;

	
	 //For performance difference tests
	//for (const auto& Primitive : mPrimitives)
	//{
	//	Primitive->IsIntersectingRay(CameraRay, &MaxTValue, &ClosestIntersection);
	//}
		
	mKDTree.IsIntersectingRay(CameraRay, &MaxTValue, &ClosestIntersection);

	// If an object was intersected
	if (ClosestIntersection.object)
	{
		// disable the current object so reflection and refraction rays don't
		// interact with it
		ClosestIntersection.object->SetEnabled(false);
		FColor OutputColor;

		// Get the surface material, point, and normal
		const Vector3f& SurfacePoint(ClosestIntersection.point);
		const Vector3f& SurfaceNormal(ClosestIntersection.normal.Normalize());
		const FMaterial& SurfaceMaterial(ClosestIntersection.object->GetMaterial(SurfacePoint));

		assert(abs(SurfaceNormal.Length() - 1.0f) < _EPSILON);

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
			else if (IsInShadow(RayToLight, light->GetDistance(SurfacePoint)))
			{
				continue;
			}

			// Get dot product of surface normal and h for specular lighting
			const float SpecularFactor = pow(std::max(Vector3f::Dot(SurfaceNormal, H), 0.f), SurfaceMaterial.GetGlossiness());

			// Get dot product of surface normal and light direction for diffuse lighting
			const float DiffuseFactor = std::max(Vector3f::Dot(SurfaceNormal, LightDirection), 0.f);

			// Combine material color and light color for diffuse and specular
			const FColor specularColor(LightColor * SurfaceMaterial.GetSpecular() * SpecularFactor);
			const FColor diffuseColor(LightColor  * SurfaceMaterial.GetDiffuse() * DiffuseFactor);

			// Add diffuse and specular contributions to total
			OutputColor += specularColor + diffuseColor;

			if (SurfaceMaterial.GetDiffuse().A < 1.0f)
			{
				OutputColor *= SurfaceMaterial.GetDiffuse().A;
				const Vector3f RefractionDirection = ComputeRefractionVector(-CameraRay.direction, SurfaceNormal, SurfaceMaterial.GetRefractiveIndex());
				assert(abs(RefractionDirection.Length() - 1) < _EPSILON);
				const FRay Refraction(SurfacePoint, RefractionDirection);

				// modify the refraction input by amount of transparency
				OutputColor += (1 - SurfaceMaterial.GetDiffuse().A) * TraceRay(Refraction, Depth - 1);
			}

			// Add mirror reflection contributions
			const Vector3f mirrorReflection = -CameraRay.direction.Reflect(SurfaceNormal);
			const FRay reflectionRay(SurfacePoint, mirrorReflection);
			OutputColor += TraceRay(reflectionRay, Depth - 1) * OutputColor * SurfaceMaterial.GetReflectivity();
			
		}

		// re-enable this object
		ClosestIntersection.object->SetEnabled(true);

		// return computed color totals with ambient contribution
		return OutputColor + (mGlobalAmbient * SurfaceMaterial.GetAmbient());
	}
	else
		return mBackgroundColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void FScene::RenderScene()
{
	// values for calculating progress of completion
	const float InvTotalPixels = 100.0f / (mOutputResolution.x * mOutputResolution.y);
	const int PercentUpdateRate = mOutputResolution.y / 20;
	int UpdateInterval = 0;

	// With supersampling
	if (mSuperSamplingLevel > 1)
	{
		for (int y = 0; y < mOutputResolution.y; y++)
		{
			for (int x = 0; x < mOutputResolution.x; x++)
			{
				FColor PixelColor;
				for (const FRay& PixelRay : mCamera.GenerateSampleRays(x, y, mSuperSamplingLevel))
				{
					PixelColor += TraceRay(PixelRay, 5);
				}

				// average the result of all samples
				PixelColor /= (float)(mSuperSamplingLevel * mSuperSamplingLevel);
				mOutputImage.SetPixel(x, y, PixelColor.Clamp());
			}

			// Display progress to console
			UpdateInterval++;
			if (UpdateInterval >= PercentUpdateRate)
			{
				UpdateInterval = 0;
				std::cout << (y * mOutputResolution.x) * InvTotalPixels << "% Complete" << std::endl;
			}
		}
	}
	// Without supersampling
	else
	{
		for (int y = 0; y < mOutputResolution.y; y++)
		{
			for (int x = 0; x < mOutputResolution.x; x++)
			{
				FColor PixelColor;
				const FRay& PixelRay = mCamera.GenerateRay(x, y);
				PixelColor = TraceRay(PixelRay, 5);
				mOutputImage.SetPixel(x, y, PixelColor.Clamp());
			}

			// Display progress to console
			UpdateInterval++;
			if (UpdateInterval >= PercentUpdateRate)
			{
				UpdateInterval = 0;
				std::cout << (y * mOutputResolution.x) * InvTotalPixels << "% Complete" << std::endl;
			}
		}
	}

	mOutputImage.WriteImage();
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

bool FScene::IsInShadow(const FRay& LightRay, float MaxDistance)
{
	return mKDTree.IsIntersectingRay(LightRay, &MaxDistance);

	// For performance tests
	//for (const auto& Primitive : mPrimitives)
	//{
	//	// If the intersects the light
	//	if (Primitive->IsIntersectingRay(LightRay, &MaxDistance))
	//		return true;
	//}

	//return false;
	
}

float FScene::ComputeShadeFactor(const ILight& Light, const Vector3f& SurfacePoint)
{
	const float FactorSize = 1.0f / mNumberOfShadowSamples;
	float ShadeFactor = 1.0f;
	const float MaxTValue = Light.GetDistance(SurfacePoint);
	for (FRay ShadowSample : Light.GetRayToLightSamples(SurfacePoint, mNumberOfShadowSamples))
	{
		// make sure the ray doesn't start below the surface
		ShadowSample.origin += ShadowSample.direction * _EPSILON;
		float tValue = MaxTValue;
		if (mKDTree.IsIntersectingRay(ShadowSample, &tValue))
		{
			ShadeFactor -= FactorSize;
		}

		//for (const auto& Primitive : mPrimitives)
		//{
		//	// If the object is not the reference one and intersects the light
		//	if (Primitive->IsIntersectingRay(ShadowSample, &MaxTValue))
		//	{
		//		ShadeFactor -= FactorSize * Primitive->GetMaterial().GetDiffuse().a;
		//		break;
		//	}
		//}
	}

	return ShadeFactor;
}
