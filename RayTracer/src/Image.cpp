#include "Image.h"
#include <sstream>
#include <limits>

FImage::FImage(const std::string& Filename, const Vector2i& OutputResolution)
	: mPixelColors(OutputResolution.y)
	, mOutputResolution(OutputResolution)
	, mFilename(Filename)
{
	// Resize each vector to the amount of vertical pixels
	for (auto& row : mPixelColors)
		row.resize(OutputResolution.x);
}

void FImage::setPixel(const int32_t& X, const int32_t& Y, const FColor& Color)
{
	mPixelColors[Y][X] = Color;
}

void FImage::writeImage()
{
	// Write to a .ppm image
	std::ostringstream headerStream;
	headerStream << "P6\n";
	headerStream << mOutputResolution.x << ' ' << mOutputResolution.y << '\n';
	headerStream << "255\n";
	std::ofstream fileStream(mFilename + ".ppm", std::ios::out | std::ios::binary);
	fileStream << headerStream.str();

	for (const auto& row : mPixelColors)
		for (const auto& pixel : row)
			fileStream << static_cast<unsigned char>(pixel.r * 255) 
				<< static_cast<unsigned char>(pixel.g * 255)
				<< static_cast<unsigned char>(pixel.b * 255);

	fileStream.flush();
	fileStream.close();
}

void FImage::setFilename(const std::string& Filename)
{
	mFilename = Filename;
}

std::string FImage::getFilename() const
{
	return mFilename;
}