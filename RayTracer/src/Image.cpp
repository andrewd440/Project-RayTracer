#include "Image.h"
#include "Windows.h"
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

void FImage::SetPixel(const int32_t& X, const int32_t& Y, const FColor& Color)
{
	mPixelColors[Y][X] = Color;
}

void FImage::WriteImage()
{
	// Write to a .ppm image
	mFilename += ".ppm";
	std::ostringstream headerStream;
	headerStream << "P6\n";
	headerStream << mOutputResolution.x << ' ' << mOutputResolution.y << '\n';
	headerStream << "255\n";
	std::ofstream fileStream(mFilename, std::ios::out | std::ios::binary);
	fileStream << headerStream.str();

	for (const auto& row : mPixelColors)
		for (const auto& pixel : row)
			fileStream << static_cast<unsigned char>(pixel.R * 255) 
				<< static_cast<unsigned char>(pixel.G * 255)
				<< static_cast<unsigned char>(pixel.B * 255);

	fileStream.flush();
	fileStream.close();

	// open the new image
	const std::wstring WFilename(mFilename.begin(), mFilename.end());
	ShellExecute(0, 0, WFilename.c_str(), 0, 0, SW_SHOW);
}

void FImage::SetFilename(const std::string& Filename)
{
	mFilename = Filename;
}

std::string FImage::GetFilename() const
{
	return mFilename;
}