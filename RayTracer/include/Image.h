#pragma once
#include "Color.h"
#include "Vector2.h"

#include <vector>

class Image
{
public:
	Image(const std::string& Filename, const Vector2i& OutputResolution);

	void setPixel(const int& X, const int& Y, const Color& Color) const;

	void setFilename(const std::string& Filename);

	std::string getFilename() const;

private:
	std::vector<Color> mPixelColors;
	std::string mFilename;
	Vector2i mOutputResolution;
};

