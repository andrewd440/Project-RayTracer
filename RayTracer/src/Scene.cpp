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
#include <map>

static const Vector2i OutputResolution(1000,600);
static std::vector<FTexture> TextureHolder;
static std::map<std::string, FMaterial> MaterialHolder;
static const uint8_t KdDepth = 10;
static const uint8_t KdMinObjects = 3;

//////////////////////////////////////////////////////////////////////////////////////////////

void throwSceneConfigError(const std::string& ObjectType)
{
	std::cout << "...Error in scene config file for a " << ObjectType << std::endl;
	throw std::runtime_error("Scene config error.");
}

//////////////////////////////////////////////////////////////////////////////////////////////
FScene::FScene()
	: mOutputImage("RenderedScene", OutputResolution)
	, mBackgroundColor(FColor::Black)
	, mGlobalAmbient(0.2f, 0.2f, 0.2f)
	, mCamera(Vector3f(0, 0, 0), Vector3f(0, 0, -1.0f), Vector3f(0, 1, 0), 75, OutputResolution)
	, mPrimitives()
	, mLights()
	, mKDTree()
	, mNumberOfShadowSamples(32)
	, mSuperSamplingLevel(2)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////

void FScene::BuildScene(std::istream& in)
{
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

			mCamera = FCamera(Position, LookAt, Up, FOV, OutputResolution);
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
			if (string != "Point:")
				throwSceneConfigError("Plane");
			in >> Point.x >> Point.y >> Point.z;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Plane");
			in >> Material;

			mPrimitives.push_back(PrimitivePtr(new FPlane(MaterialHolder[Material], Normal, Point)));
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
			if (string != "Material:")
				throwSceneConfigError("Sphere");
			in >> Material;

			mPrimitives.push_back(PrimitivePtr(new FSphere(Position, Radius, MaterialHolder[Material])));
			FMatrix4 Transform;
			Transform.SetOrigin(Position);
			Transform.Rotate(Rotation);
			mPrimitives.back()->SetTransform(Transform);
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

			mPrimitives.push_back(PrimitivePtr(new FTriangle(V0, V1, V2, MaterialHolder[Material])));
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

			mPrimitives.push_back(PrimitivePtr(new FCube(Position, MaterialHolder[Material])));
			FMatrix4 Transform;
			Transform.SetOrigin(Position);
			Transform.Rotate(Rotation);
			Transform.Scale(Scale);
			mPrimitives.back()->SetTransform(Transform);
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
		}
		in >> string;
	}

	TextureHolder.push_back(FTexture("Textures/Checker.tga"));
	TextureHolder.push_back(FTexture("Textures/Danbotexture.tga"));
	TextureHolder.push_back(FTexture("Textures/Bricks.tga"));
	//TextureHolder.push_back(FTexture("Textures/RockDiff.tga"));

	//FTextureInfo BrickPlaneInfo{ &TextureHolder[2], 0.05f, 0.05f };
	//FMaterial BrickPlaneMaterial(FColor(.3f, .1f, .1f), FColor(.8f, .2f, .2f), FColor(.1f, .1f, .1f), 1, 0.0f);
	//BrickPlaneMaterial.SetDiffuseTexture(BrickPlaneInfo);

	FTextureInfo TilePlaneInfo{ &TextureHolder[0], 0.25f, 0.25f };
	FMaterial TilePlaneMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 128, 0.8f);
	TilePlaneMaterial.SetDiffuseTexture(TilePlaneInfo);

	FTextureInfo BrickBoxInfo{ &TextureHolder[2], 0.35f, 0.25f };
	FMaterial BrickBoxMaterial(FColor(.8f, .8f, .8f), FColor(.0f, .0f, .0f), FColor(.1f, .1f, .1f), 64, 0.0f);
	BrickBoxMaterial.SetDiffuseTexture(BrickBoxInfo);

	FTextureInfo BrickSphereInfo{ &TextureHolder[2], 0.5f, 0.5f };
	FMaterial BrickSphereMaterial(FColor(.8f, .8f, .8f), FColor(.0f, .0f, .0f), FColor(.1f, .1f, .1f), 64, 0.0f);
	BrickSphereMaterial.SetDiffuseTexture(BrickSphereInfo);

	FTextureInfo BoxManTextInfo{ &TextureHolder[1], 1.0f, 1.0f };
	FMaterial BoxManMaterial(FMaterial(FColor(.7f, .7f, .7f), FColor(.7f, .7f, .7f), FColor(.1f, .1f, .1f), 12, 0.0f));
	BoxManMaterial.SetDiffuseTexture(BoxManTextInfo);
	
	//mLights.push_back(LightPtr(new FPointLight(FColor(0.6f, 0.1f, 0.4f), Vector3f(3.0f, 10.0f, -20.0f), 2, 2, 20)));
	//mLights.push_back(LightPtr(new FPointLight(FColor(0.3f, 1.0f, 1.0f), Vector3f(2.0f, 7, -10.0f), 1, 10, 35)));
	//mLights.push_back(LightPtr(new FDirectionalLight(FColor(0.3f, 0.3f, 0.3f), Vector3f(-1.0f, -1.0f, -2.0f))));

	IDrawable* Drawable;
	mPrimitives.push_back(PrimitivePtr(new FPlane(TilePlaneMaterial, Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0, -5.0f, 0))));
	//mPrimitives.push_back(PrimitivePtr(new FPlane(FMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 64, 0.1f), Vector3f(0.0f, -1.0f, 0.0f), Vector3f(0, 10.0f, 0))));
	//mPrimitives.push_back(PrimitivePtr(new FPlane(FMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 64, 0.1f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(-10.0f, 0.0f, 0))));
	//mPrimitives.push_back(PrimitivePtr(new FPlane(FMaterial(FColor(.8f, .8f, .8f), FColor(.8f, .8f, .8f), FColor(.1f, .1f, .1f), 64, 0.1f), Vector3f(-1.0f, 0.0f, 0.0f), Vector3f(10.0f, 0.0f, 0))));
	//mPrimitives.push_back(PrimitivePtr(new FPlane(FMaterial(FColor(.9f, .9f, .9f), FColor(.9f, .9f, .9f, 1.0f), FColor(.0f, .0f, .0f), 64, 1.0f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0, 0.0f, -35.0f))));
	
	//std::unique_ptr<FTriangle> Triangle(new FTriangle(Vector3f(-15.0f, 7.0f, -18.0f), Vector3f(15.0f, 8.0f, -18.0f), Vector3f(-15.0f, -11.0f, -18.0f)));
	//Triangle->SetTexture(&TextureHolder[0]);
	//Triangle->SetMaterial(FMaterial(FColor(0.0f, .0f, .0f), FColor(.1f, .1f, .1f), FColor(.1f, .1f, .1f), 12, 0.1f));
	//Triangle->SetUVCoordinates(Vector2f(0.0f, 0.0f), Vector2f(1.0f, 0.0f), Vector2f(0.0f, 1.0f));
	//mPrimitives.push_back(std::move(Triangle));

	mLights.push_back(LightPtr(new FPointLight(FColor::White, Vector3f(-3.0f, 8.0f, -6.0f), 1, 5, 35)));
	mPrimitives.push_back(PrimitivePtr(new FMesh("Models/BoxMan.obj", BoxManMaterial)));
	Drawable = mPrimitives.back().get();
	FMatrix4 trans;
	trans.SetOrigin(Vector3f(3.0f, -2.0f, -18));
	trans.Rotate(EAxis::Y, -25);
	Drawable->SetTransform(trans);

	mPrimitives.push_back(PrimitivePtr(new FMesh("Models/BoxMan.obj", BoxManMaterial)));
	Drawable = mPrimitives.back().get();
	trans = FMatrix4();
	trans.SetOrigin(Vector3f(0.0f, -3.5f, -11));
	trans.Rotate(EAxis::Y, 25);
	trans.Scale(.5f);
	Drawable->SetTransform(trans);

	mPrimitives.push_back(PrimitivePtr(new FMesh("Models/BoxMan.obj", BoxManMaterial)));
	Drawable = mPrimitives.back().get();
	trans = FMatrix4();
	trans.SetOrigin(Vector3f(-4.0f, -4.25f, -12));
	trans.Rotate(EAxis::Y, 0);
	trans.Scale(.25f);
	Drawable->SetTransform(trans);

	mPrimitives.push_back(PrimitivePtr(new FMesh("Models/BoxMan.obj", BoxManMaterial)));
	Drawable = mPrimitives.back().get();
	trans = FMatrix4();
	trans.SetOrigin(Vector3f(-2.0f, -2.75f, -22));
	trans.Rotate(EAxis::Y, 5);
	trans.Scale(.75f);
	Drawable->SetTransform(trans);
	/*mPrimitives.back().get()->Transform.SetOrigin(Vector3f(5, -1.0f, -15));
	mPrimitives.back().get()->Transform.Rotate(FMatrix4::Axis::Y, -25);*/

	//mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(-0.0f, -1.5f, -15.0f), FMaterial(FColor(.9f, .1f, .1f), FColor(.9f, .1f, .1f), FColor(.1f, .1f, .1f), 32, .1f))));
	//Drawable = mPrimitives.back().get();
	//Drawable->Transform.Rotate(FMatrix4::Axis::X, 25);
	//Drawable->Transform.Scale(Vector3f(3,.5,2));
	
	//mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(-5.0f, -2.0f, -15.0f), FMaterial(FColor(.2f, .7f, .7f), FColor(.5f, 1.0f, .5f), FColor(.1f, .1f, .1f), 64, .9f))));
	//Drawable = mPrimitives.back().get();
	//Drawable->Scale(EAxis::X, 0.5f);

	mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(-10.0f, -3.5f, -17.0f), FMaterial(FColor(.2f, .7f, .7f), FColor(.1f, .3f, .8f, 0.5f), FColor(.1f, .1f, .1f), 64, .0f))));
	Drawable = mPrimitives.back().get();
	Drawable->Scale(EAxis::Y, 1.5f);
	////Drawable->Scale(EAxis::Z, 1.5f);
	////Drawable->Scale(EAxis::X, 1.5f);

	mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(0.0f, -2.0f, -35.0f), BrickBoxMaterial)));
	Drawable = mPrimitives.back().get();
	Drawable->Scale(EAxis::Y, 1.5f);
	Drawable->Scale(EAxis::Z, 0.25f);
	Drawable->Scale(EAxis::X, 30.25f);
	//Drawable->Rotate(EAxis::Y, -10.0f);

	mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(-6.0f, -1.0f, -15.5f), 2.0f, FMaterial(FColor(.2f, .7f, .7f), FColor(1.0f, .4f, .1f, 0.3f), FColor(.1f, .1f, .1f), 64, .1f, 1.33f))));

	//mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(-2.0f, 4.0f, -20.0f), 1.0f, BrickSphereMaterial)));
	//Drawable = mPrimitives.back().get();
	//Drawable->Rotate(EAxis::Y, 25.0f);

	//mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(-5.0f, 1.0f, -15.0f), 2.0f, FMaterial(FColor(.8f, .3f, .1f), FColor(0.7f, .3f, .1f, 0.4f), FColor(.1f, .1f, .1f), 128, 0.0f, 1.4f))));
	//mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(-1.0f, 0.0f, -15.0f), 3.0f, FMaterial(FColor(0.628f, 0.555f, 0.366f), FColor(0.7516f, 0.606f, 0.226f, 1.0f), FColor(.24725f, .1995f, .0745f), 102.4f, 0.7f, 0.0f))));
	//mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(5.0f, 1.0f, -15.0f), 2.0f, FMaterial(FColor(.8f, .3f, .1f), FColor(0.7f, .3f, .1f, 0.4f), FColor(.1f, .1f, .1f), 128, 0.0f, 1.33f))));

	mKDTree.BuildTree(mPrimitives, KdDepth, KdMinObjects);
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
	const float InvTotalPixels = 100.0f / (OutputResolution.x * OutputResolution.y);
	const int PercentUpdateRate = OutputResolution.y / 20;
	int UpdateInterval = 0;

	// With supersampling
	if (mSuperSamplingLevel > 1)
	{
		for (int y = 0; y < OutputResolution.y; y++)
		{
			for (int x = 0; x < OutputResolution.x; x++)
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
				std::cout << (y * OutputResolution.x) * InvTotalPixels << "% Complete" << std::endl;
			}
		}
	}
	// Without supersampling
	else
	{
		for (int y = 0; y < OutputResolution.y; y++)
		{
			for (int x = 0; x < OutputResolution.x; x++)
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
				std::cout << (y * OutputResolution.x) * InvTotalPixels << "% Complete" << std::endl;
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

//////////////////////////////////////////////////////////////////////////////////////////////

FMaterial FScene::ReadMaterial(std::istream& input)
{
	FColor specular, diffuse, ambient;
	float specualarExponent, reflectivity;

	std::string materialProperty;
	
	input >> materialProperty;
	if (materialProperty != "Specular:")
		throwSceneConfigError("Material");
	input >> specular.R >> specular.G >> specular.B;

	input >> materialProperty;
	if (materialProperty != "Diffuse:")
		throwSceneConfigError("Material");
	input >> diffuse.R >> diffuse.G >> diffuse.B;

	input >> materialProperty;
	if (materialProperty != "Ambient:")
		throwSceneConfigError("Material");
	input >> ambient.R >> ambient.G >> ambient.B;

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